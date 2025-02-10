#include "entity.hpp"

#include "json.hpp"

#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>

extern const int MAP_WIDTH, MAP_HEIGHT;
const uint16_t NUM_ENTITIES = 100;

// extern std::unordered_map<Entity, Position> positions;
// extern std::unordered_map<Entity, Renderable> renderables;
// std::unordered_map<Entity, Health> healths;

enum AIBehavior {
    Standard,
    Aggressive,
};

void MovementSystem(Grid& grid, ECS& ecs) {
    auto& positionMap = ecs.component_manager.positionMap;
    for (Entity entity : ecs.entities()) {
        if (!ecs.has_component<Actor>(entity)) continue;
        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);

        int newX = pos->x + mov->dx;
        int newY = pos->y + mov->dy;

        if (grid.isWalkable(newX, newY)) {
            if (positionMap.find({newX, newY}) != positionMap.end()) {
                newX    -= pos->x;
                newY    -= pos->y;
                mov->dx  = 0;
                mov->dy  = 0;
            } else {
                ecs.component_manager.update_position(entity, newX, newY);
                pos->x = newX;
                pos->y = newY;
            };
        } else {
            ecs.component_manager.update_position(entity, pos->x, pos->y);
            // pos->x -= mov->dx;
            // pos->y -= mov->dy;
            mov->dx = 0;
            mov->dy = 0;
        }
    }
}

void AIMovementIntentSystem(Grid& grid, ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> std_mov(-1, 1);
    std::uniform_int_distribution<int> agg_mov(-2, 2);

    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);
    int playerX         = playerPos->x;
    int playerY         = playerPos->y;

    for (Entity entity : ecs.entities()) {
        if (entity == Player || !ecs.has_component<Actor>(entity)) continue;

        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);

        switch (*ecs.get_component<AIBehavior>(entity)) {
            case Standard:
                mov->dx = std_mov(rng);
                mov->dy = std_mov(rng);
                break;
            case Aggressive: {
                Vision vision = *ecs.get_component<Vision>(entity);
                // search for player somewhere within this entity's vision range
                if (playerX >= pos->x - vision && playerY >= pos->y - vision
                    && playerX <= pos->x + vision && playerY <= pos->y + vision) {
                    // TODO: include object collision in LOS (e.g. walls)
                    PathFindingSystem(entity, Player, grid, ecs);
                } else {
                    // randomly wander
                    mov->dx = std_mov(rng);
                    mov->dy = std_mov(rng);
                }
                break;
            }
            default: break;
        }
    }
}

void CombatSystem(ItemRegistry& itemRegistry, ECS& ecs) {
    // create map of current positions
    auto& positionMap = ecs.component_manager.positionMap;
    std::vector<Entity> to_destroy;
    for (Entity attacker : ecs.entities()) {
        if (!ecs.has_component<Actor>(attacker)) continue;
        // don't process entity if it's destroyed
        for (Entity destroyed : to_destroy)
            if (attacker == destroyed) continue;

        Position* pos = ecs.get_component<Position>(attacker);
        Movement* mov = ecs.get_component<Movement>(attacker);
        Damage* dmg   = ecs.get_component<Damage>(attacker);
        Name* attName = ecs.get_component<Name>(attacker);

        if (!(pos && mov && dmg && attName)) continue;

        int targetX = pos->x + mov->dx;
        int targetY = pos->y + mov->dy;

        auto it = positionMap.find({targetX, targetY});
        if (it == positionMap.end()) continue;

        Entity defender = it->second;
        // FIXME: shouldnt be necessary
        if (defender == attacker) continue;
        if (defender != Player && attacker != Player) continue;
        if (!ecs.has_component<Health>(defender)) continue;

        Health* defHealth = ecs.get_component<Health>(defender);
        Name* defName     = ecs.get_component<Name>(defender);

        if (!(defHealth && defName)) continue;

        // Random damage calculation
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> rand_dmg(dmg->min, dmg->max);
        int damageDealt = rand_dmg(rng);

        Inventory* attackerInv = ecs.get_component<Inventory>(attacker);
        Inventory* defenderInv = ecs.get_component<Inventory>(defender);
        if (attackerInv) {
            for (Entity item : attackerInv->items) {
                item::Weapon* weapon = ecs.get_component<item::Weapon>(item);
                if (weapon) {
                    damageDealt += weapon->damage;
                }
            }
        }
        if (defenderInv) {
            for (Entity item : defenderInv->items) {
                item::Armor* armor = ecs.get_component<item::Armor>(item);
                if (armor) {
                    damageDealt -= armor->defense;
                }
            }
        }

        // Apply damage
        defHealth->curr -= damageDealt;

        ecs.component_manager.eventLogs.push_back(
            *attName + " attacks " + *defName + " for " + std::to_string(damageDealt) + " damage!"
        );

        if (defHealth->curr <= 0) {
            to_destroy.push_back(defender);
            continue;
        }

        // Cancel movement after combat
        mov->dx = 0;
        mov->dy = 0;
    }
    for (Entity entity : to_destroy) {
        if (entity == Player) continue;
        ecs.destroy_entity(entity);
    }
}

void LineOfSightSystem(ECS& ecs) {}

int chebyshevDistance(int a, int b, int w, int h) {
    int ax = a % w;
    int ay = a / w;
    int bx = b % w;
    int by = b / w;
    int dx = ax - bx;
    int dy = ay - by;
    return (dx + dy) - 1 * std::min(dx, dy);
    // return abs(ax - bx) + abs(ay - by);
}

const double ORTHOGONAL_COST = 1.0;
const double DIAGONAL_COST   = 1.414;

const std::pair<int, int> MOVEMENT_DIRECTIONS[8] = {
    // Horizontal + Vertical
    {-1, 0 },
    {1,  0 },
    {0,  -1},
    {0,  1 },
    // Diagonals
    {-1, -1},
    {1,  -1},
    {-1, 1 },
    {1,  1 }
};

void PathFindingSystem(Entity start, Entity end, Grid& grid, ECS& ecs) {
    // start and end indices
    int s = 0, e = 0;
    Position* s_pos = ecs.get_component<Position>(start);
    Movement* s_mov = ecs.get_component<Movement>(start);
    {
        Position* e_pos = ecs.get_component<Position>(end);
        Movement* e_mov = ecs.get_component<Movement>(end);

        s = xy_idx(s_pos->x, s_pos->y);
        e = xy_idx(e_pos->x + e_mov->dx, e_pos->y + e_mov->dy);
    }
    if (s == e) return;

    int w = MAP_WIDTH, h = MAP_HEIGHT;
    int n = w * h;

    std::vector<int> came_from(n, -1);
    std::vector<double> g_score(n, std::numeric_limits<double>::infinity());
    std::vector<double> f_score(n, std::numeric_limits<double>::infinity());
    std::vector<uint8_t> visited(n, 0);

    g_score[s] = 0;
    f_score[s] = chebyshevDistance(s, e, w, h);

    auto cmp = [&](int left, int right) { return f_score[left] > f_score[right]; };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> open_set(cmp);
    open_set.push(s);

    // Run the A* algorithm
    while (!open_set.empty()) {
        int current = open_set.top();
        open_set.pop();

        // Found
        if (current == e) {
            std::vector<int> path;
            while (current != s) {
                path.push_back(current);
                current = came_from[current];
            }
            sf::Vector2i new_loc = idx_xy(path[path.size() - 1]);

            // Move entity in specified direction along calculated path
            s_mov->dx = new_loc.x - s_pos->x;
            s_mov->dy = new_loc.y - s_pos->y;
            return;
        }

        // Mark the current node as visited
        visited[current] = 1;
        int current_x    = current % w;
        int current_y    = current / w;

        // Explore neighbors
        for (auto [dx, dy] : MOVEMENT_DIRECTIONS) {
            int neighbor_x = current_x + dx;
            int neighbor_y = current_y + dy;
            int neighbor   = neighbor_y * w + neighbor_x;

            if (neighbor_x < 0 || neighbor_x >= w || neighbor_y < 0 || neighbor_y >= h) continue;
            if (!grid.isWalkable(neighbor_x, neighbor_y)) continue;

            bool is_diagonal = abs(dx) + abs(dy) == 2;
            // Prevent diagonal movement through corners
            if (is_diagonal) {                    // Diagonal move
                int adjacent1 = current + dx;     // Horizontal neighbor
                int adjacent2 = current + dy * w; // Vertical neighbor
                if (!grid.tiles[adjacent1].isWalkable() || !grid.tiles[adjacent2].isWalkable())
                    continue;
            }

            // Calculate cost
            double move_cost         = is_diagonal ? DIAGONAL_COST : ORTHOGONAL_COST;
            double tentative_g_score = g_score[current] + move_cost;

            if (tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor]   = tentative_g_score;
                f_score[neighbor]   = g_score[neighbor] + chebyshevDistance(neighbor, e, w, h);
                if (!visited[neighbor]) {
                    open_set.push(neighbor);
                }
            }
        }
    }
};

void EntityGeneratorSystem(ECS& ecs) {
    using json = nlohmann::json;

    // Read Entity Table
    std::ifstream f("entities.json");
    json data = json::parse(f);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, data.size() - 1);
    std::uniform_int_distribution<int> randx(0, MAP_WIDTH);
    std::uniform_int_distribution<int> randy(0, MAP_HEIGHT);

    // Create random entities
    using namespace rl;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        // pick random mob from entity table
        auto mob = data[picker(rng)];

        ecs.create_entity()
            .with(Actor{})
            .with(Name{mob["name"]})
            .with(Position{randx(rng), randy(rng)})
            .with(Renderable(stringSpriteMap.at(mob["name"]), stringColorMap.at(mob["color"])))
            .with(Movement{0, 0})
            .with(Health{mob["health"], mob["health"]})
            .with(Damage{mob["damage"], mob["damage"]})
            .with(Vision{mob["vision"]})
            .with(mob["behavior"] == "aggressive" ? Aggressive : Standard)
            .build();
    }
}

#include "entity.hpp"

#include "grid.hpp"

#include <iostream>
#include <random>
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

void MovementSystem(ECS& ecs) {
    for (Entity entity : ecs.entities()) {
        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);

        pos->x += mov->dx;
        pos->y += mov->dy;
    }
}

void CollisionSystem(Grid& grid, ECS& ecs) {
    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);
    for (Entity entity : ecs.entities()) {
        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);
        int dest_x    = pos->x;
        int dest_y    = pos->y;

        if (!grid.isWalkable(dest_x, dest_y)) {
            // undo movement if colliding
            pos->x -= mov->dx;
            pos->y -= mov->dy;

            mov->dx = 0;
            mov->dy = 0;
        }
    }
}

void AIMovementSystem(Grid& grid, ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> std_mov(-1, 1);
    std::uniform_int_distribution<int> agg_mov(-2, 2);

    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);
    int playerX         = playerPos->x + playerMov->dx;
    int playerY         = playerPos->y + playerMov->dy;

    for (Entity entity : ecs.entities()) {
        if (entity == Player) continue;

        Movement* mov = ecs.get_component<Movement>(entity);
        Position* pos = ecs.get_component<Position>(entity);

        switch (*ecs.get_component<AIBehavior>(entity)) {
            case Standard:
                mov->dx = std_mov(rng);
                mov->dy = std_mov(rng);
                break;
            case Aggressive: {
                Vision vision    = *ecs.get_component<Vision>(entity);
                bool playerFound = false;

                // search for player somewhere within this entity's vision range
                // TODO: include object collision in LOS (e.g. walls)
                for (int y = pos->y - vision; y < pos->y + vision; y++) {
                    for (int x = pos->x - vision; x < pos->x + vision; x++) {
                        if (x == playerX && y == playerY) {
                            playerFound = true;
                            break;
                        }
                    }
                    if (playerFound) break;
                }

                if (playerFound) {
                    // move towards the player
                    PathFindingSystem(entity, Player, grid, ecs);
                    // mov->dx = playerX > pos->x ? 1 : playerX < pos->x ? -1 : 0;
                    // mov->dy = playerY > pos->y ? 1 : playerY < pos->y ? -1 : 0;
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

void CombatSystem(ECS& ecs) {
    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);

    for (Entity entity : ecs.entities()) {
        if (entity == Player) continue;
        Movement* mov = ecs.get_component<Movement>(entity);
        Position* pos = ecs.get_component<Position>(entity);

        // if moving into the player, do combat
        if (pos->x == playerPos->x && pos->y == playerPos->y) {
            Name* attName  = ecs.get_component<Name>(entity);
            Name* defName  = ecs.get_component<Name>(Player);
            Damage* damage = ecs.get_component<Damage>(entity);
            Health* health = ecs.get_component<Health>(Player);

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<int> rand_dmg(damage->min, damage->max);
            int dmg = rand_dmg(rng);

            health->curr -= dmg;
            std::cout << *attName << " attacks " << *defName << " for " << dmg << '\n';
            std::cout << *defName << " health: " << health->curr << "/" << health->max << '\n';

            pos->x -= mov->dx;
            pos->y -= mov->dy;
        }
    }
}

void LineOfSightSystem(ECS& ecs) {}

// Helper function to calculate Manhattan distance heuristic
int heuristic(int a, int b, int w, int h) {
    int ax = a % w;
    int ay = a / w;
    int bx = b % w;
    int by = b / w;
    return abs(ax - bx) + abs(ay - by);
}

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

    int w = MAP_WIDTH, h = MAP_HEIGHT;
    int n = w * h;

    std::vector<int> came_from(n, -1);
    std::vector<int> g_score(n, std::numeric_limits<int>::max());
    std::vector<int> f_score(n, std::numeric_limits<int>::max());
    std::vector<bool> visited(n, false);

    g_score[s] = 0;
    f_score[s] = heuristic(s, e, w, h);

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
        }

        // Mark the current node as visited
        visited[current] = true;
        int current_x    = current % w;
        int current_y    = current / w;

        // Explore neighbors
        for (auto [dx, dy] : MOVEMENT_DIRECTIONS) {
            int neighbor_x = current_x + dx;
            int neighbor_y = current_y + dy;
            int neighbor   = neighbor_y * w + neighbor_x;

            // Ensure that we don't go out of bounds
            if (dx == -1 && current % w == 0) continue;    // Left, no wraparound
            if (dx == 1 && current % w == w - 1) continue; // Right, no wraparound
            if (dy == -w && current < w) continue;         // Up, no wraparound at the top
            if (dy == w && current >= n - w) continue;     // Down, no wraparound at the bottom

            // Skip OOB or walls
            {
                sf::Vector2i xy = idx_xy(neighbor);
                if (neighbor_x < 0 || neighbor_x >= w || neighbor_y < 0 || neighbor_y >= h) {
                    continue;
                }
                if (!grid.isWalkable(xy.x, xy.y)) continue;
            }

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
                f_score[neighbor]   = g_score[neighbor] + heuristic(neighbor, e, w, h);
                if (!visited[neighbor]) {
                    open_set.push(neighbor);
                }
            }
        }
    }
};

void EntityGeneratorSystem(ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, 10);
    std::uniform_int_distribution<int> randx(0, MAP_WIDTH);
    std::uniform_int_distribution<int> randy(0, MAP_HEIGHT);

    using namespace rl;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        Entity entity = ecs.create_entity();
        ecs.add_component(entity, Position{randx(rng), randy(rng)});
        ecs.add_component(entity, Movement{0, 0});
        {
            Name name;
            Renderable render;
            Health health;
            Damage damage;
            Vision vision;
            switch (picker(rng)) {
                case 0:
                    name   = "Goblin";
                    render = Renderable(GoblinUnarmed, getColor(Green));
                    health = {10, 10};
                    damage = {1, 1};
                    vision = 8;
                    break;
                case 1:
                    name   = "Goblin";
                    render = Renderable(GoblinSword, getColor(Green));
                    health = {10, 10};
                    damage = {2, 3};
                    vision = 8;
                    break;
                case 2:
                    name   = "Goblin";
                    render = Renderable(GoblinBow, getColor(Green));
                    health = {10, 10};
                    damage = {3, 4};
                    vision = 8;
                    break;
                case 3:
                    name   = "Barbarian";
                    render = Renderable(BarbarianUnarmed, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {2, 2};
                    vision = 8;
                    break;
                case 4:
                    name   = "Barbarian";
                    render = Renderable(BarbarianSword, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {3, 4};
                    vision = 8;
                    break;
                case 5:
                    name   = "Barbarian";
                    render = Renderable(BarbarianBow, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {4, 4};
                    vision = 8;
                    break;
                case 6:
                    name   = "Reptile";
                    render = Renderable(ReptileUnarmed, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {2, 2};
                    vision = 8;
                    break;
                case 7:
                    name   = "Reptile";
                    render = Renderable(ReptileSword, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {3, 4};
                    vision = 8;
                    break;
                case 8:
                    name   = "Reptile";
                    render = Renderable(ReptileBow, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {4, 4};
                    vision = 8;
                    break;
                case 9:
                    name   = "Demon";
                    render = Renderable(DemonUnarmed, getColor(Red));
                    health = {50, 10};
                    damage = {4, 4};
                    vision = 8;
                    break;
                default:
                    name   = "Demon";
                    render = Renderable(DemonAxe, getColor(Red));
                    health = {50, 10};
                    damage = {7, 7};
                    vision = 8;
                    break;
            }
            ecs.add_component(entity, name);
            ecs.add_component(entity, render);
            ecs.add_component(entity, health);
            ecs.add_component(entity, damage);
            ecs.add_component(entity, vision);
        }
        ecs.add_component(entity, AIBehavior::Aggressive);
    }
}

#include "../ecs.hpp"
#include "../json.hpp"
#include "armor.hpp"
#include "equipable.hpp"
#include "material.hpp"
#include "potion.hpp"
#include "weapon.hpp"

#include <string>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

namespace item {

// Item
struct Item {
    std::string name;
    std::string desc;
    float weight;
};

static void to_json(json& j, Item& i) {
    j = json{
        {"name",        i.name  },
        {"description", i.desc  },
        {"weight",      i.weight},
    };
}

static void from_json(const json& j, Item& i) {
    j.at("name").get_to(i.name);
    j.at("description").get_to(i.desc);
    j.at("weight").get_to(i.weight);
}

} // namespace item

struct Inventory {
    std::vector<Entity> items;
    float maxWeight     = 50.f;
    float currentWeight = 0.f;

    bool isFull() const { return currentWeight >= maxWeight; }
};

struct Equipment {
    std::unordered_map<item::Equipable::Slot, Entity> slots;
};

struct ItemRegistry {
    std::unordered_map<std::string, Entity> items;

    void register_item(const std::string& name, Entity entity) { items[name] = entity; }
    Entity get(const std::string& name) const {
        auto it = items.find(name);
        if (it != items.end()) return it->second;
        throw std::runtime_error("Item not found: " + name);
    }
    bool exists(const std::string& name) const { return items.find(name) != items.end(); }
};

void ItemGeneratorSystem(ItemRegistry&, ECS&);
void addItemToInventory(Entity owner, Entity item, ECS&);

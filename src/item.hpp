#include "ecs.hpp"
#include "json.hpp"

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

// Equipable
struct Equipable {
    enum class Slot {
        Head,
        Body,
        Leg,
        Hand,
        Foot,
        Finger,
        Neck,
        None,
    } slot;
};

/*
    Weapon
*/
struct Weapon {
    int damage;
    int range;
    float attackSpeed;
    bool isRanged;
};

static void to_json(json& j, Weapon& w) {
    j = json{
        {"damage", w.damage},
        {"range",  w.range }
    };
}

static void from_json(const json& j, Weapon& w) {
    j.at("damage").get_to(w.damage);
    j.at("range").get_to(w.range);
    j.at("attack_speed").get_to(w.attackSpeed);
    j.at("is_ranged").get_to(w.isRanged);
}

/*
    Armor
*/
struct Armor {
    int defense;
    int resistance;
};
static void to_json(json& j, Armor& a) {
    j = json{
        {"defense",    a.defense   },
        {"resistance", a.resistance}
    };
}

static void from_json(const json& j, Armor& a) {
    j.at("defense").get_to(a.defense);
    j.at("resistance").get_to(a.resistance);
}

/*
    Potion
*/
struct Potion {
    enum class Type {
        Healing,
        ManaRestore,
        Poison,
        Buff,
    } effect;
    int potency;
    int duration;
};

struct PotionClass {
    enum class Type { Lesser, Greater, Mythic };
    float potencyModifier;
    float durationModifier;
};

static const std::unordered_map<PotionClass::Type, PotionClass> POTION_CLASS_DATABASE = {
    {PotionClass::Type::Lesser,  {1.f, 1.f}},
    {PotionClass::Type::Greater, {2.f, 2.f}},
    {PotionClass::Type::Mythic,  {4.f, 4.f}},
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    Potion::Type,
    {
        {Potion::Type::Healing,     "healing"     },
        {Potion::Type::ManaRestore, "mana_restore"},
        {Potion::Type::Poison,      "poison"      },
        {Potion::Type::Buff,        "buff"        },
}
)

static void to_json(json& j, Potion& p) {
    j = json{
        {"effect",   p.effect  },
        {"potency",  p.potency },
        {"duration", p.duration}
    };
}

static void from_json(const json& j, Potion& p) {
    j.at("potency").get_to(p.potency);
    j.at("duration").get_to(p.duration);
    j.at("effect").get_to(p.effect);
}

struct Material {
    enum class Type {
        Wood,
        Bronze,
        Iron,
        Steel,
        Mithril,
    };
    int durabilityModifier;
    float weightModifier;
    float damageModifier;
};

static const std::unordered_map<Material::Type, Material> MATERIAL_DATABASE = {
    {Material::Type::Wood,    {50, 1.0f, 0.9f} },
    {Material::Type::Bronze,  {80, 1.5f, 1.0f} },
    {Material::Type::Iron,    {100, 2.0f, 1.1f}},
    {Material::Type::Steel,   {150, 3.0f, 1.3f}},
    {Material::Type::Mithril, {250, 5.0f, 1.5f}},
};

NLOHMANN_JSON_SERIALIZE_ENUM(
    Material::Type,
    {
        {Material::Type::Wood,    "wood"   },
        {Material::Type::Bronze,  "bronze" },
        {Material::Type::Iron,    "iron"   },
        {Material::Type::Steel,   "steel"  },
        {Material::Type::Mithril, "mithril"},
}
)

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

std::string materialToString(item::Material::Type type);
item::Potion::Type stringToPotionEffect(std::string s);
item::PotionClass::Type stringToPotionClass(std::string s);
item::Equipable stringToEquipable(std::string s);

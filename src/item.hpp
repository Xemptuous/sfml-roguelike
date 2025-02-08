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

typedef std::vector<Item*> Inventory;

// Equipable
struct Equipable {
    enum class Slot {
        Head,
        Body,
        Hand,
        Finger,
        Neck,
        None,
    } slot;
};

static const std::unordered_map<std::string, Equipable> equipableMap = {
    {"head",   Equipable{Equipable::Slot::Head}  },
    {"body",   Equipable{Equipable::Slot::Body}  },
    {"hand",   Equipable{Equipable::Slot::Hand}  },
    {"finger", Equipable{Equipable::Slot::Finger}},
    {"neck",   Equipable{Equipable::Slot::Neck}  },
    {"none",   Equipable{Equipable::Slot::None}  },
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
static const std::unordered_map<std::string, Potion::Type> potionEffectMap = {
    {"healing",      Potion::Type::Healing    },
    {"mana_restore", Potion::Type::ManaRestore},
    {"poison",       Potion::Type::Poison     },
    {"buff",         Potion::Type::Buff       },
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
    {Material::Type::Wood,    {50, 0.8f, 0.9f} },
    {Material::Type::Bronze,  {80, 1.0f, 1.0f} },
    {Material::Type::Iron,    {100, 1.2f, 1.1f}},
    {Material::Type::Steel,   {150, 1.5f, 1.3f}},
    {Material::Type::Mithril, {250, 2.0f, 1.5f}},
};

static const std::unordered_map<std::string, Material> materialMap = {
    {"wood",    MATERIAL_DATABASE.at(Material::Type::Wood)   },
    {"bronze",  MATERIAL_DATABASE.at(Material::Type::Bronze) },
    {"iron",    MATERIAL_DATABASE.at(Material::Type::Iron)   },
    {"steel",   MATERIAL_DATABASE.at(Material::Type::Steel)  },
    {"mithril", MATERIAL_DATABASE.at(Material::Type::Mithril)},
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

void ItemGeneratorSystem(ECS& ecs);

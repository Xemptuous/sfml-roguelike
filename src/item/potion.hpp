#include "../json.hpp"
using json = nlohmann::json;

namespace item {

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
} // namespace item

static const item::Potion::Type stringToPotionEffect(std::string s) {
    using namespace item;
    if (s == "healing") return Potion::Type::Healing;
    else if (s == "mana") return Potion::Type::ManaRestore;
    else if (s == "poison") return Potion::Type::Poison;
    else if (s == "buff") return Potion::Type::Buff;
    return Potion::Type::Buff;
}

static const item::PotionClass::Type stringToPotionClass(std::string s) {
    using namespace item;
    if (s == "lesser") return PotionClass::Type::Lesser;
    else if (s == "greater") return PotionClass::Type::Lesser;
    else return PotionClass::Type::Mythic;
}

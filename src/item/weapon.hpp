#include "../json.hpp"
using json = nlohmann::json;

namespace item {

struct Weapon {
    enum class Type {
        Edged,
        Blunt,
    };
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

// NLOHMANN_JSON_SERIALIZE_ENUM(
//     Weapon::Type,
//     {
//         {Weapon::Type::Edged, "edged"},
//         {Weapon::Type::Blunt, "blunt"},
// }
// )
} // namespace item

// static const item::Weapon::Type stringToWeaponType(std::string s) {
//     using namespace item;
//     if (s == "lesser") return Weapon::Type::Edged;
//     else if (s == "greater") return Weapon::Type::Blunt;
//     return Weapon::Type::Edged;
// }

#include "../json.hpp"
using json = nlohmann::json;

namespace item {

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

static const std::string materialToString(item::Material::Type type) {
    using item::Material;
    switch (type) {
        case Material::Type::Wood:    return "wood";
        case Material::Type::Bronze:  return "bronze";
        case Material::Type::Iron:    return "iron";
        case Material::Type::Steel:   return "steel";
        case Material::Type::Mithril: return "mithril";
    }
    return "wood";
}

#include "../json.hpp"
using json = nlohmann::json;

namespace item {

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
} // namespace item

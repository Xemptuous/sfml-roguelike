#include "../json.hpp"
using json = nlohmann::json;

namespace item {

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

} // namespace item

static const item::Equipable stringToEquipable(std::string s) {
    using namespace item;
    if (s == "head") return Equipable{Equipable::Slot::Head};
    else if (s == "body") return Equipable{Equipable::Slot::Body};
    else if (s == "leg") return Equipable{Equipable::Slot::Leg};
    else if (s == "hand") return Equipable{Equipable::Slot::Hand};
    else if (s == "foot") return Equipable{Equipable::Slot::Foot};
    else if (s == "finger") return Equipable{Equipable::Slot::Finger};
    else if (s == "neck") return Equipable{Equipable::Slot::Neck};
    else return Equipable{Equipable::Slot::None};
}

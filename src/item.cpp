#include "item.hpp"

#include "json.hpp"

#include <fstream>

void ItemGeneratorSystem(ItemRegistry& registry, ECS& ecs) {
    using json = nlohmann::json;
    using namespace item;
    std::ifstream f("items.json");
    json data = json::parse(f);

    // generate non-potions
    for (const json& item : data) {
        for (const auto& [materialType, materialData] : MATERIAL_DATABASE) {
            Item baseItem = item.get<Item>();
            if (item.contains("potion")) continue;

            ECS::EntityBuilder builder = ecs.create_entity();

            baseItem.name    = materialToString(materialType) + " " + baseItem.name;
            baseItem.weight *= materialData.weightModifier;
            builder.with(baseItem);

            // // Item types
            if (item.contains("weapon")) {
                Weapon weapon  = item["weapon"].get<Weapon>();
                weapon.damage *= materialData.damageModifier;
                builder.with(weapon);
            } else if (item.contains("armor")) {
                Armor armor    = item["armor"].get<Armor>();
                armor.defense *= materialData.damageModifier;
                builder.with(item["armor"].get<Armor>());
            }

            if (item.contains("equipable")) {
                for (json equipable : item["equipable"]) {
                    builder.with(stringToEquipable(equipable));
                }
            }
            Entity entity = builder.build();
            registry.register_item(baseItem.name, entity);
        }

        // // generate potions
        // for (const auto& [potClassType, potClassData] : POTION_CLASS_DATABASE) {
        //     Item baseItem = item.get<Item>();
        // }
    }
}

void addItemToInventory(Entity owner, Entity item, ECS& ecs) {
    using namespace item;
    Inventory* inventory = ecs.get_component<Inventory>(owner);
    if (inventory && !inventory->isFull()) {
        Item* i = ecs.get_component<Item>(item);
        if (i->weight + inventory->currentWeight <= inventory->maxWeight) {
            inventory->items.push_back(item);
            inventory->currentWeight += i->weight;
        }
    }
}
item::Potion::Type stringToPotionEffect(std::string s) {
    using namespace item;
    if (s == "healing") return Potion::Type::Healing;
    else if (s == "mana") return Potion::Type::ManaRestore;
    else if (s == "poison") return Potion::Type::Poison;
    else if (s == "buff") return Potion::Type::Buff;
    return Potion::Type::Buff;
}

item::PotionClass::Type stringToPotionClass(std::string s) {
    using namespace item;
    if (s == "lesser") return PotionClass::Type::Lesser;
    else if (s == "greater") return PotionClass::Type::Lesser;
    else return PotionClass::Type::Mythic;
}

item::Equipable stringToEquipable(std::string s) {
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

std::string materialToString(item::Material::Type type) {
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

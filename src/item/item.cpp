#include "item.hpp"

#include <fstream>

void ItemGeneratorSystem(ItemRegistry& registry, ECS& ecs) {
    using json = nlohmann::json;
    using namespace item;
    std::ifstream f("src/items/items.json");
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

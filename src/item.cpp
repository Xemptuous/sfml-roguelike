#include "item.hpp"

#include "json.hpp"

#include <fstream>

void ItemGeneratorSystem(ECS& ecs) {
    using json = nlohmann::json;
    using namespace item;
    std::ifstream f("items.json");
    json data = json::parse(f);

    for (json item : data) {
        ECS::EntityBuilder builder = ecs.create_entity();

        // Required Base
        builder.with(item.template get<Item>());

        // Item types
        if (item.contains("weapon")) {
            builder.with(item["weapon"].template get<Weapon>());
        } else if (item.contains("armor")) {
            builder.with(item["armor"].template get<Armor>());
        } else if (item.contains("potion")) {
            printf("POTION CREATED\n");
            builder.with(item["potion"].template get<Potion>());
        }

        // Additional fields
        if (item.contains("material")) {
            builder.with(materialMap.at(item["material"]));
        }
        if (item.contains("equipable")) {
            for (json equipable : item["equipable"]) {
                builder.with(equipableMap.at(equipable));
            }
        }
        builder.build();
    }
}

#include "grid.hpp"

#include <SFML/Graphics/Sprite.hpp>

enum EntityType {
    Player,
    Mob,
    ShopKeeper,
};

struct Entity {
    EntityType type;
    sf::Sprite* sprite;
    Position position;

    Entity(EntityType type) : type(type) {};
    Entity(Position position, EntityType type) : type(type), position(position) {};
};

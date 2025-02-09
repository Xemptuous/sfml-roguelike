#pragma once
#include "ecs.hpp"

#include <SFML/Graphics.hpp>

extern bool show_inventory;

void UISystem(sf::RenderWindow& window, sf::Font& font, ECS& ecs);

void showInventory(sf::RenderWindow& window, sf::Font& font, ECS& ecs);

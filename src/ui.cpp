#include "ui.hpp"

#include "entity.hpp"
#include "item/item.hpp"
#include "sprite.hpp"

#include <cmath>

extern int SPRITE_WIDTH, SPRITE_HEIGHT;
extern int RENDER_WIDTH, RENDER_HEIGHT;

void UISystem(RenderWindow& window, Font& font, ECS& ecs) {
    // draw base
    unsigned int rh       = 400;
    unsigned int rw       = RENDER_WIDTH;
    unsigned int rect_top = RENDER_HEIGHT - rh;
    RectangleShape rect({(float)rw, (float)rh});
    rect.setPosition({0.f, (float)rect_top});
    rect.setFillColor(Color::Black);
    rect.setOutlineColor(Color::White);
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    float top_margin = 20;

    // Player HP
    Health* hp = ecs.get_component<Health>(Player);
    Text php(font);
    php.setString("Player HP: " + std::to_string(hp->curr) + "/" + std::to_string(hp->max));
    php.setCharacterSize(32);
    php.setFillColor(Color::White);
    php.setStyle(Text::Bold);
    php.setPosition({20.f, rect_top + top_margin});
    window.draw(php);

    // Event Log
    // TODO: add "scrolling" to the logs to fit in screen
    // also consider sizing based on window
    std::vector<std::string>& logs = ecs.component_manager->eventLogs;

    int line_height   = 26;
    float left_margin = 900;

    int i = 0;
    for (std::string event : ecs.component_manager->eventLogs) {
        Text log(font);
        log.setString(event);
        log.setCharacterSize(32);
        log.setFillColor(Color::White);
        log.setStyle(Text::Regular);
        log.setPosition({rw - left_margin, rect_top + top_margin + line_height * i});
        i++;
        window.draw(log);
    }
}

void RenderInventory(sf::RenderWindow& window, sf::Font font, ECS& ecs) {
    Inventory* inventory = ecs.get_component<Inventory>(Player);

    // sf::Sprite sprite(SPRITE_SHEET);

    int fontSize = 18;
    int yOffset  = 20;
    char label   = 'a';
    for (Entity entity : inventory->items) {
        item::Item* item     = ecs.get_component<item::Item>(entity);
        SpriteTiles tileType = stringSpriteMap.at(item->type);

        // Draw the label and item name
        sf::Text icon(font, itemCharMap.at(item->type), fontSize);
        icon.setPosition(Vector2f(25, yOffset));

        // item::Material* material = ecs.get_component<item::Material>(entity);
        // if (material) {
        //     icon.setFillColor(MATERIAL_COLORS.at(material))
        // }

        sf::Text text(font, std::string(1, label) + ") " + item->name, fontSize);
        text.setPosition(Vector2f(40, yOffset));
        window.draw(icon);
        window.draw(text);

        yOffset += SPRITE_HEIGHT + 20;
        label++;
    }
}

void showInventory(RenderWindow& window, Font& font, ECS& ecs) {
    float width  = 1200;
    float height = 900;
    Vector2f center((float)RENDER_WIDTH / 2, (float)RENDER_HEIGHT / 2);

    int text_margin = 24;

    sf::RectangleShape rect(Vector2f(width, height));
    // rect.setOrigin(center);
    rect.setPosition(Vector2f(center.x - width / 2, center.y - height / 2));
    rect.setFillColor(Color::Black);
    rect.setOutlineColor(Color::Yellow);
    rect.setOutlineThickness(2.0);

    sf::Text title(font, "Inventory", 32);
    title.setOrigin(title.getLocalBounds().size / 2.f + title.getLocalBounds().position);
    title.setPosition(
        rect.getPosition() + rect.getSize() / 2.f - Vector2f{0.f, height / 2 - text_margin}
    );
    auto rectGlobalBounds = rect.getGlobalBounds();
    auto rectLocalBounds  = rect.getLocalBounds();

    auto textGlobalBounds = title.getGlobalBounds();
    auto textLocalBounds  = title.getLocalBounds();

    title.setFillColor(Color::White);
    window.draw(rect);
    window.draw(title);

    if (ecs.has_component<Inventory>(Player)) {

        Inventory* inv    = ecs.get_component<Inventory>(Player);
        int line_height   = 30;
        float left_margin = 900;

        int i = 2;
        for (Entity invItem : inv->items) {
            item::Item* item = ecs.get_component<item::Item>(invItem);
            Text itemText(font, item->name, 32);
            itemText.setFillColor(Color::White);
            itemText.setStyle(Text::Regular);
            itemText.setOrigin(
                itemText.getLocalBounds().size / 2.f + itemText.getLocalBounds().position
            );
            itemText.setPosition(
                rect.getPosition() + rect.getSize() / 2.f
                - Vector2f{0.f, height / 2 - text_margin - i * line_height}
            );
            i++;
            window.draw(itemText);
        }
    }
}

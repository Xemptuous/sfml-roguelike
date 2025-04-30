#include "ui.hpp"

#include "entity.hpp"
#include "item/item.hpp"
#include "item/material.hpp"
#include "sprite.hpp"

#include <cmath>

extern int SPRITE_WIDTH, SPRITE_HEIGHT;
extern int RENDER_WIDTH, RENDER_HEIGHT;

void UISystem(RenderWindow& window, Font& font, ECS& ecs) {
    const Vector2f viewCenter = window.getView().getCenter();
    const Vector2f viewSize   = window.getView().getSize();

    const unsigned int rectHeight = 200;

    const int viewLeft   = viewCenter.x - viewSize.x / 2;
    const int viewRight  = viewCenter.x + viewSize.x / 2;
    const int viewTop    = viewCenter.y - viewSize.y / 2;
    const int viewBottom = viewCenter.y + viewSize.y / 2;
    const int hMargin    = 5;
    const int vMargin    = 2;

    {
        sf::RectangleShape rect({viewSize.x, (float)rectHeight});
        rect.setOrigin({0, 0});
        rect.setPosition({(float)viewLeft, (float)viewBottom - rectHeight});
        rect.setFillColor(sf::Color::Black);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1.f);
        window.draw(rect);
    }

    // Player HP
    const Health* hp = ecs.get_component<Health>(Player);
    Text php(font, "Player HP: " + std::to_string(hp->curr) + "/" + std::to_string(hp->max), 32);
    php.setStyle(Text::Bold);
    php.setPosition(Vector2f(viewLeft, viewBottom - rectHeight + vMargin));
    window.draw(php);

    std::vector<std::string>& logs = ecs.component_manager->eventLogs;

    const int line_height = 30;
    const int num_lines   = 6;

    size_t longestLine = 0;
    for (std::string txt : logs) {
        longestLine = std::max(longestLine, txt.size());
    }

    int start = 0;
    for (int i = logs.size() >= num_lines ? logs.size() - num_lines : 0; i < logs.size(); i++) {
        Text log(font, logs[i], 24);
        // log.setFillColor(sf::Color::White);
        // log.setStyle(Text::Bold);
        // log.setStyle(Text::Regular);
        log.setPosition(Vector2f(
            viewRight - log.getLocalBounds().size.length(),
            viewBottom - rectHeight + vMargin + line_height * start
        ));
        start++;
        window.draw(log);
    }
}

void RenderInventory(sf::RenderWindow& window, sf::Font font, ECS& ecs) {
    Inventory* inventory = ecs.get_component<Inventory>(Player);

    // sf::Sprite sprite(SPRITE_SHEET);
    // // rect.setOrigin(center);
    // rect.setPosition({0, 0});
    // rect.setFillColor(sf::Color::Black);
    // // rect.setOutlineColor(Color::Yellow);
    // rect.setOutlineThickness(2.0);
    // window.draw(rect);

    // window.draw(rect);
    Vector2f viewCenter = window.getView().getCenter();
    Vector2f viewSize   = window.getView().getSize();

    int viewLeft = viewCenter.x - viewSize.x / 2;
    int viewTop  = viewCenter.y - viewSize.y / 2;

    sf::RectangleShape rect({0, 0});
    Vector2f rectPos(viewLeft, viewTop);
    rect.setPosition(rectPos);
    rect.setFillColor(sf::Color::Black);

    std::vector<Text> toDraw;
    float max_size = 0;
    int fontSize   = 20;
    int yOffset    = 0;
    char ch        = 'a';
    for (Entity entity : inventory->items) {
        item::Item* item     = ecs.get_component<item::Item>(entity);
        SpriteTiles tileType = stringSpriteMap.at(item->type);

        sf::Text label(font, std::string(1, ch) + ") ", fontSize);
        label.setPosition(Vector2f(viewLeft + 5, viewTop + yOffset));

        // Draw the label and item name
        sf::Text icon(font, itemCharMap.at(item->type), fontSize);
        icon.setPosition(Vector2f(viewLeft + 35, viewTop + yOffset));

        item::Material::Type* materialType = ecs.get_component<item::Material::Type>(entity);
        if (materialType) {
            icon.setFillColor(item::MATERIAL_COLORS.at(*materialType));
        }

        sf::Text text(font, item->name, fontSize);
        text.setPosition(Vector2f(viewLeft + 50, viewTop + yOffset));
        max_size = std::max(
            max_size, label.getLocalBounds().size.length() + icon.getLocalBounds().size.length()
                          + text.getLocalBounds().size.length()
        );
        toDraw.push_back(label);
        toDraw.push_back(icon);
        toDraw.push_back(text);
        yOffset += SPRITE_HEIGHT + 20;
        ch++;
    }
    rect.setSize(Vector2f(max_size, yOffset));
    window.draw(rect);

    for (Text txt : toDraw) {
        window.draw(txt);
    }
}

// void showInventory(RenderWindow& window, Font& font, ECS& ecs) {
//     float width  = 1200;
//     float height = 900;
//     Vector2f center((float)RENDER_WIDTH / 2, (float)RENDER_HEIGHT / 2);
//
//     int text_margin = 24;
//
//     sf::RectangleShape rect(Vector2f(width, height));
//     // rect.setOrigin(center);
//     rect.setPosition(Vector2f(center.x - width / 2, center.y - height / 2));
//     rect.setFillColor(Color::Black);
//     rect.setOutlineColor(Color::Yellow);
//     rect.setOutlineThickness(2.0);
//
//     sf::Text title(font, "Inventory", 32);
//     title.setOrigin(title.getLocalBounds().size / 2.f + title.getLocalBounds().position);
//     title.setPosition(
//         rect.getPosition() + rect.getSize() / 2.f - Vector2f{0.f, height / 2 - text_margin}
//     );
//     auto rectGlobalBounds = rect.getGlobalBounds();
//     auto rectLocalBounds  = rect.getLocalBounds();
//
//     auto textGlobalBounds = title.getGlobalBounds();
//     auto textLocalBounds  = title.getLocalBounds();
//
//     title.setFillColor(Color::White);
//     window.draw(rect);
//     window.draw(title);
//
//     if (ecs.has_component<Inventory>(Player)) {
//
//         Inventory* inv    = ecs.get_component<Inventory>(Player);
//         int line_height   = 30;
//         float left_margin = 900;
//
//         int i = 2;
//         for (Entity invItem : inv->items) {
//             item::Item* item = ecs.get_component<item::Item>(invItem);
//             Text itemText(font, item->name, 32);
//             itemText.setFillColor(Color::White);
//             itemText.setStyle(Text::Regular);
//             itemText.setOrigin(
//                 itemText.getLocalBounds().size / 2.f + itemText.getLocalBounds().position
//             );
//             itemText.setPosition(
//                 rect.getPosition() + rect.getSize() / 2.f
//                 - Vector2f{0.f, height / 2 - text_margin - i * line_height}
//             );
//             i++;
//             window.draw(itemText);
//         }
//     }
// }

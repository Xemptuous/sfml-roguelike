#include <SFML/Graphics/Color.hpp>

namespace rl {
enum Color {
    White,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Cyan,
    Magenta,

    Silver,
    Gray,
    DarkRed,
    DarkGreen,
    DarkYellow,
    DarkBlue,
    DarkCyan,
    DarkMagenta,
    SandyBrown,
    Tan,
    SaddleBrown,
};
}

static const sf::Color COLOR_ARRAY[19]{
    sf::Color::White,
    sf::Color::Black,
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Blue,
    sf::Color::Cyan,
    sf::Color::Magenta,

    sf::Color(192, 192, 192, 255), // Silver
    sf::Color(64, 64, 64, 255),    // Gray
    sf::Color(64, 0, 0, 255),      // Dark Red
    sf::Color(0, 64, 0, 255),      // Dark Green
    sf::Color(64, 64, 0, 255),     // Dark Yellow
    sf::Color(0, 0, 64, 255),      // Dark Blue
    sf::Color(0, 64, 64, 255),     // Dark Cyan
    sf::Color(64, 0, 64, 255),     // Dark Magenta

    sf::Color(244, 164, 96, 255),  // SandyBrown
    sf::Color(210, 180, 140, 255), // Tan
    sf::Color(139, 69, 19, 255),   // SaddleBrown
};

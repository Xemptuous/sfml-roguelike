#include <SFML/Graphics/Color.hpp>
#include <string>
#include <unordered_map>

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

    BlackOpaque,
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

static const sf::Color COLOR_ARRAY[20]{
    sf::Color::White,
    sf::Color::Black,
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Blue,
    sf::Color::Cyan,
    sf::Color::Magenta,

    sf::Color(0, 0, 0, 128),       // BlackOpaque
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

static sf::Color getColor(rl::Color color) {
    return COLOR_ARRAY[color];
}

static const std::unordered_map<std::string, sf::Color> stringColorMap = {
    {"green",        COLOR_ARRAY[rl::Green]      },
    {"red",          COLOR_ARRAY[rl::Red]        },
    {"sandy_brown",  COLOR_ARRAY[rl::SandyBrown] },
    {"white",        COLOR_ARRAY[rl::White]      },
    {"black",        COLOR_ARRAY[rl::Black]      },
    {"red",          COLOR_ARRAY[rl::Red]        },
    {"green",        COLOR_ARRAY[rl::Green]      },
    {"yellow",       COLOR_ARRAY[rl::Yellow]     },
    {"blue",         COLOR_ARRAY[rl::Blue]       },
    {"cyan",         COLOR_ARRAY[rl::Cyan]       },
    {"magenta",      COLOR_ARRAY[rl::Magenta]    },
    {"black_opaque", COLOR_ARRAY[rl::BlackOpaque]},
    {"silver",       COLOR_ARRAY[rl::Silver]     },
    {"gray",         COLOR_ARRAY[rl::Gray]       },
    {"dark_red",     COLOR_ARRAY[rl::DarkRed]    },
    {"dark_green",   COLOR_ARRAY[rl::DarkGreen]  },
    {"dark_yellow",  COLOR_ARRAY[rl::DarkYellow] },
    {"dark_blue",    COLOR_ARRAY[rl::DarkBlue]   },
    {"dark_cyan",    COLOR_ARRAY[rl::DarkCyan]   },
    {"dark_magenta", COLOR_ARRAY[rl::DarkMagenta]},
    {"sandy_brown",  COLOR_ARRAY[rl::SandyBrown] },
    {"tan",          COLOR_ARRAY[rl::Tan]        },
    {"saddle_brown", COLOR_ARRAY[rl::SaddleBrown]},
};

static sf::Color strToColor(std::string s) {
    using namespace rl;
    if (s == "green") return COLOR_ARRAY[Green];
    if (s == "red") return COLOR_ARRAY[Red];
    if (s == "sandy_brown") return COLOR_ARRAY[SandyBrown];
    if (s == "white") return COLOR_ARRAY[White];
    if (s == "black") return COLOR_ARRAY[Black];
    if (s == "red") return COLOR_ARRAY[Red];
    if (s == "green") return COLOR_ARRAY[Green];
    if (s == "yellow") return COLOR_ARRAY[Yellow];
    if (s == "blue") return COLOR_ARRAY[Blue];
    if (s == "cyan") return COLOR_ARRAY[Cyan];
    if (s == "magenta") return COLOR_ARRAY[Magenta];
    if (s == "black_opaque") return COLOR_ARRAY[BlackOpaque];
    if (s == "silver") return COLOR_ARRAY[Silver];
    if (s == "gray") return COLOR_ARRAY[Gray];
    if (s == "dark_red") return COLOR_ARRAY[DarkRed];
    if (s == "dark_green") return COLOR_ARRAY[DarkGreen];
    if (s == "dark_yellow") return COLOR_ARRAY[DarkYellow];
    if (s == "dark_blue") return COLOR_ARRAY[DarkBlue];
    if (s == "dark_cyan") return COLOR_ARRAY[DarkCyan];
    if (s == "dark_magenta") return COLOR_ARRAY[DarkMagenta];
    if (s == "sandy_brown") return COLOR_ARRAY[SandyBrown];
    if (s == "tan") return COLOR_ARRAY[Tan];
    if (s == "saddle_brown") return COLOR_ARRAY[SaddleBrown];
    return COLOR_ARRAY[Black];
}

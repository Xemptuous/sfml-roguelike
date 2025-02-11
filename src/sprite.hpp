#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <unordered_map>
#include <vector>

extern sf::Texture SPRITE_SHEET;
extern sf::Image SPRITE_SHEET_IMAGE;

using namespace sf;

enum SpriteTiles : uint16_t {
    // Inorganic Static
    None,
    Block,
    BrownWall1,
    BrownWall2,
    BrownWall3,
    BrownWallCracked1,
    BrownWallCracked2,
    BrownWallCracked3,
    WoodWall1,
    StoneWall1,
    DoorClosed1,
    Door2,
    Door3,
    Door4,
    Stone1,
    Stone2,
    Wood1,
    StoneWindow1,
    StoneWindow2,
    Carpet1,
    Carpet2,
    Carpet3,
    Carpet4,

    // Organic Static
    Grass1,
    Grass2,
    DimGrass1,
    DimGrass2,
    Flower1,
    Flower2,
    Mushroom1,
    Mushroom2,
    Tree1,
    Tree2,

    // Entities
    PlayerMaleStanding,
    PlayerMaleWalking1,
    PlayerMaleWalking2,
    GoblinUnarmed,
    GoblinSword,
    GoblinBow,
    BarbarianUnarmed,
    BarbarianSword,
    BarbarianBow,
    ReptileUnarmed,
    ReptileSword,
    ReptileBow,
    DemonUnarmed,
    DemonAxe,

    // Items
    Potion,
    Scroll,
    Food,
    Wand,
    Staff,
    Ring,
    Amulet,
    Gold,
    Polearm,
    EdgedWeapon,
    BluntWeapon,
    RangedWeapon,
    Ammo,
    SoftArmor,
    HardArmor,
    MiscArmor,
    Shield,

    COUNT,
};

constexpr size_t NUM_SPRITES = static_cast<size_t>(SpriteTiles::COUNT);

static const std::unordered_map<SpriteTiles, Vector2i> SPRITE_COORDINATES = {
    // (x, y) from upperleft
    // Inorganic Static
    {None,               Vector2i({17, 0})},
    {Block,              Vector2i({9, 41})},
    {BrownWall1,         Vector2i({0, 0}) },
    {BrownWall2,         Vector2i({1, 0}) },
    {BrownWall3,         Vector2i({2, 0}) },
    {BrownWallCracked1,  Vector2i({3, 0}) },
    {BrownWallCracked2,  Vector2i({4, 0}) },
    {BrownWallCracked3,  Vector2i({5, 0}) },
    {WoodWall1,          Vector2i({2, 4}) },
    {StoneWall1,         Vector2i({7, 2}) },
    {DoorClosed1,        Vector2i({4, 1}) },
    {Door2,              Vector2i({1, 1}) },
    {Door3,              Vector2i({2, 1}) },
    {Door4,              Vector2i({3, 1}) },
    {Stone1,             Vector2i({0, 2}) },
    {Stone2,             Vector2i({1, 2}) },
    {Wood1,              Vector2i({2, 4}) },
    {StoneWindow1,       Vector2i({3, 2}) },
    {StoneWindow2,       Vector2i({4, 2}) },
    {Carpet1,            Vector2i({4, 5}) },
    {Carpet2,            Vector2i({2, 6}) },
    {Carpet3,            Vector2i({8, 5}) },
    {Carpet4,            Vector2i({8, 6}) },

    // Organic Static
    {Grass1,             Vector2i({3, 9}) },
    {Grass2,             Vector2i({4, 9}) },
    {DimGrass1,          Vector2i({15, 5})},
    {DimGrass2,          Vector2i({16, 5})},
    {Flower1,            Vector2i({0, 10})},
    {Flower2,            Vector2i({1, 10})},
    {Mushroom1,          Vector2i({2, 10})},
    {Mushroom2,          Vector2i({3, 10})},
    {Tree1,              Vector2i({4, 10})},
    {Tree2,              Vector2i({5, 10})},

    // Entities
    {PlayerMaleStanding, Vector2i({0, 16})},
    {GoblinUnarmed,      Vector2i({26, 2})},
    {GoblinSword,        Vector2i({27, 2})},
    {GoblinBow,          Vector2i({28, 2})},
    {BarbarianUnarmed,   Vector2i({26, 3})},
    {BarbarianSword,     Vector2i({27, 3})},
    {BarbarianBow,       Vector2i({28, 3})},
    {ReptileUnarmed,     Vector2i({26, 4})},
    {ReptileSword,       Vector2i({28, 4})},
    {ReptileBow,         Vector2i({29, 4})},
    {DemonUnarmed,       Vector2i({26, 5})},
    {DemonAxe,           Vector2i({27, 5})},
};

static const std::unordered_map<SpriteTiles, Vector2i> ASCII_COORDINATES = {
    // (x, y) from upperleft
    // Inorganic Static
    {None,               Vector2i({17, 0}) },
    {BrownWall1,         Vector2i({11, 13})},
    {WoodWall1,          Vector2i({11, 13})},
    {StoneWall1,         Vector2i({11, 13})},
    {DoorClosed1,        Vector2i({11, 2}) },
    {Stone1,             Vector2i({14, 2}) },
    {Carpet1,            Vector2i({14, 2}) },
    {Stone2,             Vector2i({14, 2}) },
    {Carpet2,            Vector2i({14, 2}) },
    {Carpet3,            Vector2i({14, 2}) },
    {Carpet4,            Vector2i({14, 2}) },
    {Wood1,              Vector2i({14, 2}) },

    // Organic Static
    {Grass1,             Vector2i({12, 2}) }, // ,
    {DimGrass1,          Vector2i({12, 2}) }, // ,
    {Grass2,             Vector2i({14, 2}) }, // .
    {DimGrass2,          Vector2i({14, 2}) }, // .
    {Flower1,            Vector2i({10, 2}) }, // *
    {Flower2,            Vector2i({10, 2}) }, // *
    {Mushroom1,          Vector2i({12, 2}) }, // ,
    {Mushroom2,          Vector2i({14, 2}) }, // .
    {Tree1,              Vector2i({11, 2}) }, // +
    {Tree2,              Vector2i({11, 2}) }, // +

    // Entities
    {PlayerMaleStanding, Vector2i({0, 4})  }, // @
    {GoblinUnarmed,      Vector2i({7, 6})  }, // g
    {GoblinSword,        Vector2i({7, 6})  }, // g
    {GoblinBow,          Vector2i({7, 6})  }, // g
    {BarbarianUnarmed,   Vector2i({2, 6})  }, // b
    {BarbarianSword,     Vector2i({2, 6})  }, // b
    {BarbarianBow,       Vector2i({2, 6})  }, // b
    {ReptileUnarmed,     Vector2i({12, 6}) }, // l
    {ReptileSword,       Vector2i({12, 6}) }, // l
    {ReptileBow,         Vector2i({12, 6}) }, // l
    {DemonUnarmed,       Vector2i({4, 6})  }, // d
    {DemonAxe,           Vector2i({4, 6})  }, // d

    // Items
    {Potion,             Vector2i({1, 2})  }, // !
    {Scroll,             Vector2i({15, 3}) }, // ?
    {Food,               Vector2i({12, 2}) }, // ,
    {Wand,               Vector2i({13, 2}) }, // -
    {Staff,              Vector2i({15, 5}) }, // _
    {Ring,               Vector2i({13, 4}) }, // =
    {Amulet,             Vector2i({2, 2})  }, // "
    {Gold,               Vector2i({3, 2})  }, // $
    {Polearm,            Vector2i({15, 2}) }, // /
    {EdgedWeapon,        Vector2i({12, 7}) }, // |
    {BluntWeapon,        Vector2i({12, 5}) }, // backslash
    {RangedWeapon,       Vector2i({13, 7}) }, // }
    {Ammo,               Vector2i({11, 7}) }, // {
    {SoftArmor,          Vector2i({8, 2})  }, // (
    {HardArmor,          Vector2i({11, 5}) }, // [
    {MiscArmor,          Vector2i({13, 5}) }, // ]
    {Shield,             Vector2i({9, 2})  }, // )
};

static const std::unordered_map<std::string, SpriteTiles> stringSpriteMap = {
    {"barbarian",          BarbarianUnarmed  },
    {"goblin",             GoblinUnarmed     },
    {"demon",              DemonUnarmed      },
    {"none",               None              },
    {"block",              Block             },
    {"brownWall1",         BrownWall1        },
    {"brownWall2",         BrownWall2        },
    {"brownWall3",         BrownWall3        },
    {"brownWallCracked1",  BrownWallCracked1 },
    {"brownWallCracked2",  BrownWallCracked2 },
    {"brownWallCracked3",  BrownWallCracked3 },
    {"woodWall1",          WoodWall1         },
    {"stoneWall1",         StoneWall1        },
    {"doorClosed1",        DoorClosed1       },
    {"door2",              Door2             },
    {"door3",              Door3             },
    {"door4",              Door4             },
    {"stone1",             Stone1            },
    {"stone2",             Stone2            },
    {"wood1",              Wood1             },
    {"stoneWindow1",       StoneWindow1      },
    {"stoneWindow2",       StoneWindow2      },
    {"carpet1",            Carpet1           },
    {"carpet2",            Carpet2           },
    {"carpet3",            Carpet3           },
    {"carpet4",            Carpet4           },
    {"grass1",             Grass1            },
    {"grass2",             Grass2            },
    {"dimGrass1",          DimGrass1         },
    {"dimGrass2",          DimGrass2         },
    {"flower1",            Flower1           },
    {"flower2",            Flower2           },
    {"mushroom1",          Mushroom1         },
    {"mushroom2",          Mushroom2         },
    {"tree1",              Tree1             },
    {"tree2",              Tree2             },
    {"playerMaleStanding", PlayerMaleStanding},
    {"playerMaleWalking1", PlayerMaleWalking1},
    {"playerMaleWalking2", PlayerMaleWalking2},
    {"goblinUnarmed",      GoblinUnarmed     },
    {"goblinSword",        GoblinSword       },
    {"goblinBow",          GoblinBow         },
    {"barbarianUnarmed",   BarbarianUnarmed  },
    {"barbarianSword",     BarbarianSword    },
    {"barbarianBow",       BarbarianBow      },
    {"reptileUnarmed",     ReptileUnarmed    },
    {"reptileSword",       ReptileSword      },
    {"reptileBow",         ReptileBow        },
    {"demonUnarmed",       DemonUnarmed      },
    {"demonAxe",           DemonAxe          },
    {"potion",             Potion            },
    {"scroll",             Scroll            },
    {"food",               Food              },
    {"wand",               Wand              },
    {"staff",              Staff             },
    {"ring",               Ring              },
    {"amulet",             Amulet            },
    {"gold",               Gold              },
    {"polearm",            Polearm           },
    {"edged_weapon",       EdgedWeapon       },
    {"blunt_weapon",       BluntWeapon       },
    {"ranged_weapon",      RangedWeapon      },
    {"ammo",               Ammo              },
    {"soft_armor",         SoftArmor         },
    {"hard_armor",         HardArmor         },
    {"misc_armor",         MiscArmor         },
    {"shield",             Shield            },
};

static const std::unordered_map<std::string, char> itemCharMap = {
    {"potion",        '!' },
    {"scroll",        '?' },
    {"food",          ',' },
    {"wand",          '-' },
    {"staff",         '_' },
    {"ring",          '=' },
    {"amulet",        '"' },
    {"gold",          '$' },
    {"polearm",       '/' },
    {"edged_weapon",  '|' },
    {"blunt_weapon",  '\\'},
    {"ranged_weapon", '}' },
    {"ammo",          '{' },
    {"soft_armor",    '(' },
    {"hard_armor",    '[' },
    {"misc_armor",    ']' },
    {"shield",        ')' },
};

static std::vector<Sprite> SPRITE_REGISTRY(NUM_SPRITES, Sprite(SPRITE_SHEET));

int SpritesheetLoadingSystem();
void SpriteGenerator();
Sprite* getSpriteTile(SpriteTiles tile);
SpriteTiles strToSpriteTile(std::string c);

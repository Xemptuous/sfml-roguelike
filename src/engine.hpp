#include "grid.hpp"
#include "player.hpp"

struct ViewRect {
    int x1; // Top left
    int y1; // Top Left
    int x2; // Bottom Right
    int y2; // Bottom Right
};

struct Options {
    bool is_ascii;
};

struct Engine {
    Player* player;
    Grid* grid;
    sf::Vector2u screen_size;
    sf::Vector2f scale_factor;
    sf::Vector2f size_factor;
    ViewRect view_rect;

    Options options;

    Engine(Options);
    ~Engine();

    void handleKey();

    void initMap();
    void updateMap();
    void drawMap();
    void swapTilesets();

    void drawEntities();

    void update();
    void updateScreenSize();
    void updateViewPosition();
    void updateViewDimensions();
    void tryMoveView(int dx, int dy);

    Vector2f coordsToPosition(int x, int y);
    Vector2i positionToCoords(float x, float y);

    void tryMovePlayer(int dx, int dy);
};

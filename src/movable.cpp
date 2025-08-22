#include "movable.h"
#include "gamelevel.h"

void Movable::MoveBy(float dx, float dy) {
    TmxMap* map = gameLevel.GetMap();

    position.x += dx;
    position.y += dy;

    // clip the position to ensure it stays within the bounds of the map
    if (position.x < 0) 
    {
        position.x = 0;
    }
    if (position.x > map->width * map->tileWidth - GetRect().width)
    {
        position.x = map->width * map->tileWidth - GetRect().width;
    }
    if (position.y > map->height * map->tileHeight - GetRect().height) {
        position.y = map->height * map->tileHeight - GetRect().height;
        velocity.y = 0; // reset vertical velocity when clamped to bottom
    }
}
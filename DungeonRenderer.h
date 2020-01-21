#pragma once

#include <vector>

#include "Location.h"
#include "Level.h"

class DungeonRenderer
{
    int pawnSpeed;

public:
    void drawMap();
    void createEmptyMap(Vector2 size);
    void initializeLevel(Dungeon dungeon);
    bool checkForKey(Vector2 position);
    void placeAThing(Vector2 position, char thing);
    Vector2 moveAThing(MoveDirection direction, Vector2 position);

    void unlockDoor();
    bool pawnHasExited(Vector2 position);
    void setPawnSpeed(int pawnSpeed_)
    {
        pawnSpeed = pawnSpeed_;
    }

    std::vector<std::vector<char>> getMap()
    {
        return map;
    }

    Vector2 exitPosition;
private:
    std::vector<std::vector<char>> map;
    std::vector<char> setHorizontalBorder(int width);


    void put(DungeonFeat feat);
    void put();
    void placeRoom(Vector2 topLeft, Vector2 bottomRight);
    void placeHorizontalHallway(Vector2 topLeft, Vector2 bottomRight);
    void placeVerticalHallway(Vector2 topLeft, Vector2 bottomRight);
    void placeExit(Vector2 position);

    Vector2 attemptMove(Vector2 oldLocation, Vector2 newLocation);
};
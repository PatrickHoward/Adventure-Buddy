#pragma once

#include <vector>
#include <algorithm>

#include "Location.h"
#include "Object.h"

enum DungeonElements
{
    ROOM,
    VHALL, //Vertical Hallway
    HHALL, //Horizontal Hallway
    EXIT,
};

struct DungeonFeat
{
    DungeonFeat(DungeonElements element_, Vector2 topLeft_, Vector2 bottomRight_)
        : element(element_),
        topLeft(topLeft_),
        bottomRight(bottomRight_)
    {

    }

    DungeonElements element;
    Vector2 topLeft;
    Vector2 bottomRight;
};

class Dungeon
{
public:
    Dungeon(Vector2 size_, Key key_, Vector2 playerStart_)
        : size(size_),
        levelKey(key_),
    playerStart(playerStart_)
    {
        
    }

    void addFeat(DungeonFeat feat)
    {
        feats.push_back(feat);
    }

    std::vector<DungeonFeat> getFeats()
    {
        return feats;
    }

    Vector2 getSize()
    {
        return size;
    }

    Key getKey()
    {
        return levelKey;
    }

    Vector2 playerStart;
private:
    Vector2 size;
    std::vector<DungeonFeat> feats;

    Key levelKey;
};
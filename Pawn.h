#pragma once

#include <vector>

#include "Location.h"
#include "Actions.h"
#include "DungeonRenderer.h"
#include "Pathfind.h"

void SetStatusMessage(const char* str);

class PlayerPawn
{
public:
    PlayerPawn(Vector2 position_, int speed_)
        : position(position_),
        symbol('o'),
        speed(speed_)
    {
        hasKey = false;
    }

    int speed;

    char symbol;

    virtual void onInstantiate(std::vector<std::vector<char>>& map_, Vector2 keyLoc, Vector2 exitLoc) = 0;
    virtual void onDestruct() = 0;
    virtual Action getAction(DungeonRenderer& renderer) = 0;

    virtual Vector2 getPosition()
    {
        return position;
    }

    virtual void setPosition(Vector2 newPosition)
    {
        position = newPosition;
    }

    virtual bool possesesKey()
    {
        return hasKey;
    }

    virtual void collectedKey()
    {
        hasKey = true;
    }

protected:

    Vector2 position;
    bool hasKey;

};

class Human : public PlayerPawn
{
public:
    Human(Vector2 position_)
        : PlayerPawn(position_, 2)
    {

    }

    int speed;

    void onInstantiate(std::vector<std::vector<char>>& map_, Vector2 keyLoc, Vector2 exitLoc) override
    {

    }

    void onDestruct() override
    {

    }

    Action getAction(DungeonRenderer& renderer) override
    {
        char value = _getch();

        switch (value)
        {
        case 'q':
            return Action::Quit;
        case 'w':
            return Action::MoveNorth;
        case 's':
            return Action::MoveSouth;
        case 'a':
            return Action::MoveWest;
        case 'd':
            return Action::MoveEast;
        case 't':
            return Action::TakeKey;
        default:
            return Action::None;
        }
    }

private:


};

class FsmBot : public PlayerPawn
{
public:
    FsmBot(Vector2 position_)
        : PlayerPawn(position_, 1)
    {
        state = FindKey;
        map = NULL;

        //Fill pathmap with solid walls.
        for (int i = 0; i < 100; ++i)
        {
            std::vector<int> row;
            for (int j = 0; j < 100; ++j)
            {
                row.push_back(0);
            }

            pathMap.push_back(row);
        }
    }

    int speed;

    void onInstantiate(std::vector<std::vector<char>>& map_, Vector2 keyLoc, Vector2 exitLoc) override
    {
        calculatePathmap(map_);
        keyPath = getPath(keyLoc);
    }

    void onDestruct() override
    {

    }

    Action getAction(DungeonRenderer& renderer) override
    {
        Sleep(25);
        auto map = renderer.getMap();

        Action action;
        Vector2 exitLocation;

        switch (state)
        {
        case FindKey:
            SetStatusMessage("Bot: I need to find Key");
            action = nextActionOnPathFind(keyPath, map);

            if(action == TakeKey)
            {
                state = GetKey;
            }

            return action;

        case GetKey:
            SetStatusMessage("Bot: I need to take Key");

            hasKey = true;
            state = FindExit;
            calculatePathmap(map);

            exitLocation = renderer.exitPosition;
            ++exitLocation.x;

            pathMap[exitLocation.y][exitLocation.x] = 1;
            pathMap[exitLocation.y + 1][exitLocation.x] = 1;
            pathMap[exitLocation.y][exitLocation.x + 1] = 1;
            pathMap[exitLocation.y + 1][exitLocation.x + 1] = 1;

            exitPath = getPath(exitLocation);

            return TakeKey;
        case FindExit:
            SetStatusMessage("Bot: I need to find Exit");
            return nextActionOnPathFind(exitPath, map);
        }
    }

private:
    std::vector<std::vector<int>> pathMap;
    std::vector<std::vector<char>>* map;

    std::vector<Vector2> keyPath;
    std::vector<Vector2> exitPath;

    botStates state;

    Action nextActionOnPathFind(std::vector<Vector2>& path, std::vector<std::vector<char>>& map_)
    {
        if (path.size() == 2)
        {
            auto next = path[1];

            if (map_[next.y][next.x] == 'k')
            {
                return TakeKey;
            }
        }

        if(path.size() <= 1)
        {
            return None;
        }

        MoveDirection movement;
        Vector2  direction =  path[1] - path[0];

        path.erase(path.begin());

        //Vector2 direction(position.x - destination.x, position.y - destination.y);

        movement = directionBasedOnVector2(direction);

        switch (movement)
        {
        case NORTH:
            return MoveNorth;
        case SOUTH:
            return MoveSouth;
        case EAST:
            return MoveEast;
        case WEST:
            return MoveWest;
        case NORTHEAST:
            return MoveNorthEast;
        case SOUTHEAST:
            return MoveSouthEast;
        case NORTHWEST:
            return MoveNorthWest;
        case SOUTHWEST:
            return MoveSouthWest;
        case STANDSTILL:
            return None;
        }

        return None;
    }

    MoveDirection directionBasedOnVector2(Vector2 direction)
    {
        if (direction.x > 0)
        {
            return EAST;
        }
        else if (direction.x < 0)
        {
            return WEST;
        }

        if (direction.y > 0)
        {
            return SOUTH;
        }
        else if (direction.y < 0)
        {
            return NORTH;
        }

        return STANDSTILL;
    }

    void calculatePathmap(std::vector<std::vector<char>>& map)
    {
        for (int i = 0; i < map.size(); ++i)
        {
            for (int j = 0; j < map[0].size(); ++j)
            {
                if (map[i][j] == '=' || map[i][j] == '|' || map[i][j] == '[' || map[i][j] == ']' || map[i][j] == 'I' || map[i][j] == 'X' || map[i][j] == '-')
                {
                    pathMap[i][j] = 0;
                }
                else
                {
                    pathMap[i][j] = 1;
                }
            }
        }
    }

    std::vector<Vector2> getPath(Vector2 destination)
    {
        auto path = pathByBFS(pathMap, position, destination);

        if(path.size() == 0)
        {
            return path;
        }

        return path;
    }

};
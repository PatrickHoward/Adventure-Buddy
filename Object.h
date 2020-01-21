#pragma once
#include "Location.h"

class GameObject
{
public:
    GameObject(Vector2 position_, char symbol_)
        : position(position_),
        symbol(symbol_)
    {

    }

    char symbol;
    virtual void onCollect() = 0;
    virtual void onUpdate() = 0;
    virtual void onDestruct() = 0;
    virtual void onInstansiate() = 0;

    Vector2 getPosition()
    {
        return position;
    }

    void setPosition(Vector2 newPosition)
    {
        position = newPosition;
    }

private:
    Vector2 position;

};

class Key : public GameObject
{
public:
    Key(Vector2 position_, char symbol_)
        : GameObject(position_, symbol_)
    {

    }

    void onCollect() override
    {

    }

    void onUpdate() override
    {

    }

    void onDestruct() override
    {

    }

    void onInstansiate() override
    {

    }

};

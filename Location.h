#pragma once

enum MoveDirection
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTHWEST,
    NORTHEAST,
    SOUTHWEST,
    SOUTHEAST,
    STANDSTILL,
};

struct Vector2
{
    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(int singleValue)
        : x(singleValue),
        y(singleValue)
    {
        
    }

    Vector2(int x_, int y_)
        : x(x_),
        y(y_)
    {

    }

    Vector2 operator-(const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }

    bool operator==(Vector2 other) const
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(Vector2 other) const
    {
        return (!this->operator==(other));
    }

    long x;
    long y;
};

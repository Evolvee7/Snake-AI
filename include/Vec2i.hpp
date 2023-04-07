#pragma once



struct Vec2i
{
    int x;
    int y;

    bool operator == (const Vec2i& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator != (const Vec2i& other) const
    {
        return !(*this == other);
    }
};
#include "Utilities.hpp"



bool are_opposite(Direction dir1, Direction dir2)
{
    if(dir1 == Direction::DOWN && dir2 == Direction::UP)
        return true;
    if(dir1 == Direction::UP && dir2 == Direction::DOWN)
        return true;
    if(dir1 == Direction::LEFT && dir2 == Direction::RIGHT)
        return true;
    if(dir1 == Direction::RIGHT && dir2 == Direction::LEFT)
        return true;

    return false;
}

Vec2i get_position_in_front(const Vec2i& pos, Direction dir)
{
    if(dir == Direction::UP)
        return Vec2i{pos.x, pos.y-1};
    if(dir == Direction::DOWN)
        return Vec2i{pos.x, pos.y+1};
    if(dir == Direction::LEFT)
        return Vec2i{pos.x-1, pos.y};
    if(dir == Direction::RIGHT)
        return Vec2i{pos.x+1, pos.y};
    return pos;
}
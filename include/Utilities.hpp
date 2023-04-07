#pragma once

#include "Direction.hpp"
#include "Vec2i.hpp"



bool are_opposite(Direction dir1, Direction dir2);
Vec2i get_position_in_front(const Vec2i& pos, Direction dir);
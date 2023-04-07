#pragma once

#include "Direction.hpp"
#include "Snake.hpp"
#include "Vec2i.hpp"



class SnakeAI
{
public:
    Direction CalculateMoveDir(const Snake& snake, const Vec2i& pellet_pos)
    {
        Direction move_dir = Direction::NONE;

        if(pellet_pos.x < snake.GetHeadPos().x)
        {
            if(!snake.WillCollide(LEFT))
                move_dir = LEFT;
            else if(!snake.WillCollide(RIGHT))
                move_dir = RIGHT;
            else if(!snake.WillCollide(UP))
                move_dir = UP;
            else if(!snake.WillCollide(DOWN))
                move_dir = DOWN;
        }
        else if(pellet_pos.x > snake.GetHeadPos().x)
        {
            if(!snake.WillCollide(RIGHT))
                move_dir = RIGHT;
            else if(!snake.WillCollide(LEFT))
                move_dir = LEFT;
            else if(!snake.WillCollide(UP))
                move_dir = UP;
            else if(!snake.WillCollide(DOWN))
                move_dir = DOWN;
        }
        else if(pellet_pos.y < snake.GetHeadPos().y)
        {
            if(!snake.WillCollide(UP))
                move_dir = UP;
            else if(!snake.WillCollide(RIGHT))
                move_dir = RIGHT;
            else if(!snake.WillCollide(LEFT))
                move_dir = LEFT;
            else if(!snake.WillCollide(DOWN))
                move_dir = DOWN;
        }
        else if(pellet_pos.y > snake.GetHeadPos().y)
        {
            if(!snake.WillCollide(DOWN))
                move_dir = DOWN;
            else if(!snake.WillCollide(UP))
                move_dir = UP;
            else if(!snake.WillCollide(RIGHT))
                move_dir = RIGHT;
            else if(!snake.WillCollide(LEFT))
                move_dir = LEFT;
        }

        return move_dir;
    }
};
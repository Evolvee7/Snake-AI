#pragma once

#include "Direction.hpp"
#include "Vec2i.hpp"
#include <SDL2/SDL_render.h>



class Snake
{
public:
    Snake(const Vec2i& pos);

    void Move();
    void Grow();
    void Draw(SDL_Renderer* renderer);

    void SetMoveDir(Direction dir) { move_dir = dir; }
    void SetLength(int value) { length = value; }

    bool WillCollide(Direction dir) const;
    bool IsCollision(const Vec2i& pos) const;
    int GetLength() const { return length; }
    Direction GetMoveDir() const { return move_dir; }
    const Vec2i& GetHeadPos() const { return body[0]; }
    const Vec2i& GetBackPos() const { return body[length-1]; }
    const Vec2i* GetPositions() const { return body; }

private:
    Vec2i body[16*9];
    int length = 1;
    Direction move_dir;

    Vec2i& head = body[0];
};
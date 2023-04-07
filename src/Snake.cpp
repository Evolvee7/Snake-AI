#include "Snake.hpp"
#include "Utilities.hpp"



Snake::Snake(const Vec2i& pos)
{
    head = pos;
    move_dir = Direction::NONE;
}

void Snake::Move()
{
    // Move body
    for(int i = length-1; i > 0; i--)
    {
        body[i] = body[i-1];
    }

    // Move head
    head = get_position_in_front(head, move_dir);

    // Limit head position to screen
    if(head.x < 0)
        head.x = 15;
    else if(head.x > 15)
        head.x = 0;
    if(head.y < 0)
        head.y = 8;
    else if(head.y > 8)
        head.y = 0;
}

void Snake::Grow()
{
    body[length] = GetBackPos();
    length++;
}

void Snake::Draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 180, 0, SDL_ALPHA_OPAQUE);

    SDL_Rect rect{0, 0, 50, 50};
    for(int i = 0; i < length; i++)
    {
        rect.x = body[i].x*50;
        rect.y = body[i].y*50;
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Snake::WillCollide(Direction dir) const
{
    const Vec2i next_snake_pos = get_position_in_front(head, dir);
    
    if(next_snake_pos != GetBackPos() && IsCollision(next_snake_pos))
        return true;
    return false;
}

bool Snake::IsCollision(const Vec2i& pos) const
{
    for(int i = 0; i < length; i++)
    {
        if(pos == body[i])
            return true;
    }
    return false;
}
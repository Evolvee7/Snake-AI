#pragma once

#include "Vec2i.hpp"
#include <SDL2/SDL_render.h>



class Game;

class Pellet
{
public:
    Pellet(Game* game);
    void Reposition();
    void Draw(SDL_Renderer* renderer);

    const Vec2i& GetPos() const { return pos; }

private:
    Vec2i pos;

    Game* game;
};
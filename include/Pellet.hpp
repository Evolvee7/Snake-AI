#pragma once

#include "Vec2i.hpp"
#include "Window.hpp"



class Game;

class Pellet
{
public:
    Pellet(Game* game);
    void Reposition();
    void Draw(Window& window);

    const Vec2i& GetPos() const { return pos; }

private:
    Vec2i pos;

    Game* game;
};
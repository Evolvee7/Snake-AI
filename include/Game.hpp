#pragma once

#include "Pellet.hpp"
#include "Snake.hpp"
#include <memory>
#include <SDL2/SDL.h>



class Game
{
public:
    Game(int screen_w, int screen_h, const char* title);
    ~Game();

    void Run();
    void OnWin();
    void OnGameOver();

    const std::unique_ptr<Vec2i[]> GetUnoccupiedPositions(int& count) const;

private:
    void Render();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Snake snake;
    Pellet pellet;
};
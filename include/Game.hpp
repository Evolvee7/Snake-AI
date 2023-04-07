#pragma once

#include "Pellet.hpp"
#include "Window.hpp"
#include "Snake.hpp"
#include "SnakeAI.hpp"
#include <memory>



class Game
{
public:
    Game(const WindowData& window_data);
    ~Game();

    void Run();
    void OnWin();
    void OnGameOver();

    const std::unique_ptr<Vec2i[]> GetUnoccupiedPositions(int& count) const;

private:
    void Render();

private:
    Window window;
    
    SnakeAI ai;
    Snake snake;
    Pellet pellet;
};
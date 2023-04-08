#pragma once

#include "Pellet.hpp"
#include "Window.hpp"
#include "Snake.hpp"
#include <memory>



class Game
{
public:
    Game(const WindowData& window_data);
    ~Game();

    void Reset();

    void Run();
    void OnWin();
    void OnGameOver();

    const Snake& GetSnake() const { return snake; }
    const Pellet& GetPellet() const { return pellet; }
    Direction GetMoveDir() const { return next_move_dir; }

    const std::unique_ptr<Vec2i[]> GetUnoccupiedPositions(int& count) const;

private:
    void Render();

private:
    Window window;
    
    Snake snake;
    Pellet pellet;

    Direction next_move_dir;
};
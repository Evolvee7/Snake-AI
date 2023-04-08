#include "Game.hpp"
#include "Utilities.hpp"
#include "Agent.hpp"
#include <SDL2/SDL.h>



Game::Game(const WindowData& window_data):
snake(Vec2i{4,4}), pellet(this), window(window_data)
{
    snake.Grow();
    pellet.Reposition();
}

Game::~Game()
{
    SDL_Quit();
}

void Game::Reset()
{
    snake.SetLength(2);
    snake.SetHeadPos({4,4});
    pellet.Reposition();
}

void Game::Run()
{
    constexpr uint32_t move_delay_ms = 50;
    uint32_t start_ms = SDL_GetTicks();
    next_move_dir = Direction::RIGHT;
    SDL_Event e;

    Agent agent(this);

    while(true)
    {
        if(SDL_TICKS_PASSED(SDL_GetTicks() - move_delay_ms, start_ms))
        {
            start_ms = SDL_GetTicks();

            next_move_dir = agent.GetNextDirection();
            
            snake.SetMoveDir(next_move_dir);
            snake.Move();

            agent.TeachModel();

            if(snake.IsSelfCollision())
            {
                OnGameOver();
                Reset();
            }

            // Should snake do om nom nom?
            if(snake.GetHeadPos() == pellet.GetPos())
            {
                snake.Grow();
                if(snake.GetLength() == 16*9) // win condition
                {
                    OnWin();
                    Reset();
                }
                else
                    pellet.Reposition();
            }
            Render();
        }

        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                return;
            }
        }
    }
}

void Game::OnWin()
{
    Vec2i display_positions[]{
        {0,0},                          {4,0},      {6,0},      {8,0},                  {11,0},
        {0,1},          {2,1},          {4,1},      {6,1},      {8,1},  {9,1},          {11,1},
        {0,2},  {1,2},          {3,2},  {4,2},      {6,2},      {8,2},          {10,2}, {11,2},
        {0,3},                          {4,3},      {6,3},      {8,3},                  {11,3}
    };
    Vec2i offset{2,2};

    window.SetDrawColor(50, 180, 50);
    window.Clear();
    window.SetDrawColor(0, 50, 0);
    SDL_Rect rect{0, 0, 50, 50};
    for(const Vec2i& pos : display_positions)
    {
        rect.x = (offset.x + pos.x)*50;
        rect.y = (offset.y + pos.y)*50;
        window.DrawRect(rect);
    }
    window.Present();

    // Hold win screen for some time
    const uint32_t start_ms = SDL_GetTicks();
    while(SDL_TICKS_PASSED(SDL_GetTicks() - 3000, start_ms) == false);
}

void Game::OnGameOver()
{
    constexpr int anim_time_ms = 2000;
    const int delay_ms = anim_time_ms/snake.GetLength();
    uint32_t start_ms = SDL_GetTicks();

    while(snake.GetLength() > 0)
    {
        // Delay every step
        if(SDL_TICKS_PASSED(SDL_GetTicks() - delay_ms, start_ms))
        {
            start_ms = SDL_GetTicks();

            snake.SetLength(snake.GetLength() - 1);
            Render();
        }
    }
}

const std::unique_ptr<Vec2i[]> Game::GetUnoccupiedPositions(int& count) const
{
    // Find all occupied positions
    bool occupied[16*9];

    Vec2i body_pos;
    for(int i = 0; i < snake.GetLength(); i++)
    {
        body_pos = snake.GetPositions()[i];

        occupied[body_pos.y*16 + body_pos.x] = true;
    }
    occupied[pellet.GetPos().y*16, pellet.GetPos().x] = true;

    // Count unoccupied positions
    count = 0;
    for(int i = 0; i < 16*9; i++)
        if(occupied[i] == false)
            count++;
    
    // Create array for unoccupied positions
    std::unique_ptr<Vec2i[]> unoccupied = std::make_unique<Vec2i[]>(16*9);
    int i = 0;
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 16; x++)
        {
            if(occupied[y*16 + x] == false)
            {
                unoccupied[i] = Vec2i{x,y};
                i++;
            }
        }
    }

    return unoccupied;
}

void Game::Render()
{
    // Draw background
    window.SetDrawColor(120, 120, 120);
    window.Clear();

    window.SetDrawColor(0, 0, 0);
    window.DrawGrid({0,0}, {16,9}, {50,50});

    // Render all actors
    pellet.Draw(window);
    snake.Draw(window);

    // Display
    window.Present();
}
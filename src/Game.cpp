#include "Game.hpp"
#include "Utilities.hpp"



Game::Game(int screen_w, int screen_h, const char* title):
snake(Vec2i{4,4}), pellet(this)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(screen_w, screen_h, 0, &window, &renderer);
    SDL_SetWindowTitle(window, title);

    pellet.Reposition();
}

Game::~Game()
{
    if(renderer)
        SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Run()
{
    constexpr uint32_t move_delay_ms = 50;
    uint32_t start_ms = SDL_GetTicks();
    Direction next_move_dir = Direction::RIGHT;
    SDL_Event e;

    while(true)
    {
        if(SDL_TICKS_PASSED(SDL_GetTicks() - move_delay_ms, start_ms))
        {
            start_ms = SDL_GetTicks();

            next_move_dir = ai.CalculateMoveDir(snake, pellet.GetPos());

            // Prevent movement in opposite direction
            if(are_opposite(snake.GetMoveDir(), next_move_dir))
                next_move_dir = snake.GetMoveDir();
            
            // Predict self-collision
            if(snake.WillCollide(next_move_dir)) // gameover condition
            {
                OnGameOver();
                return;
            }

            snake.SetMoveDir(next_move_dir);
            snake.Move();

            // Should snake do om nom nom?
            if(snake.GetHeadPos() == pellet.GetPos())
            {
                snake.Grow();
                if(snake.GetLength() == 16*9) // win condition
                {
                    OnWin();
                    return;
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
            else if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        return;
                    case SDLK_q:
                        return;
                }
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

    SDL_SetRenderDrawColor(renderer, 50, 180, 50, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect{0, 0, 50, 50};
    for(const Vec2i& pos : display_positions)
    {
        rect.x = (offset.x + pos.x)*50;
        rect.y = (offset.y + pos.y)*50;
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);

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
    SDL_SetRenderDrawColor(renderer, 120, 120, 120, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // Make mesh-like structure
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect{0, 0, 50, 50};
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 16; x++)
        {
            rect.x = x * 50;
            rect.y = y * 50;
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    // Render all actors
    pellet.Draw(renderer);
    snake.Draw(renderer);

    // Display
    SDL_RenderPresent(renderer);
}
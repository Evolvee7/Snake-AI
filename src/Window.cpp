#include "Window.hpp"
#include <SDL2/SDL.h>



Window::Window(const WindowData& data)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(data.scr_w, data.scr_h, 0, &window, &renderer);
    SDL_SetWindowTitle(window, data.title);
}

Window::~Window()
{
    if(renderer)
        SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);
}

void Window::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::Present()
{
    SDL_RenderPresent(renderer);
}

void Window::Clear()
{
    SDL_RenderClear(renderer);
}

void Window::DrawGrid(const Vec2i& pos, const Vec2i& cells, const Vec2i& cell_size)
{
    SDL_Rect rect{0,0,cell_size.x, cell_size.y};
    for(int y = 0; y < cells.y; y++)
    {
        for(int x = 0; x < cells.x; x++)
        {
            rect.x = x*cell_size.x + pos.x;
            rect.y = y*cell_size.y + pos.y;
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
}

void Window::DrawRect(const SDL_Rect& rect)
{
    SDL_RenderDrawRect(renderer, &rect);
}

void Window::FillRect(const SDL_Rect& rect)
{
    SDL_RenderFillRect(renderer, &rect);
}
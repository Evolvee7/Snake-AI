#pragma once

#include "Vec2i.hpp"
#include <SDL2/SDL_render.h>



struct WindowData
{
    int scr_w;
    int scr_h;
    const char* title;
};

class Window
{
public:
    Window(const WindowData& data);
    ~Window();

    void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = SDL_ALPHA_OPAQUE);
    void Present();
    void Clear();

    void DrawGrid(const Vec2i& pos, const Vec2i& cells, const Vec2i& cell_size);
    void DrawRect(const SDL_Rect& rect);
    void FillRect(const SDL_Rect& rect);

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};
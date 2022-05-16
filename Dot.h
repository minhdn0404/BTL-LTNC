
#pragma once
#include <SDL.h>
#include "tools.h"
#include "SDL_util.h"
using namespace std;

struct Dot
{
    int x;
    int y;
    int x0;
    int y0;
    int size = DOT_SIZE; // kích thước dot
    int step = STEP;
    int directionX = step;
    int directionY = 0;
    SDL_Texture* snake_head;
    Dot ()
    {
        snake_head = nullptr;
    }
    ~Dot()
    {
        SDL_DestroyTexture(snake_head);
        snake_head = nullptr;
    }
    Dot(int _x, int _y)
    {
        x = _x;
        y = _y;
        snake_head = nullptr;
    }
    void draw (Uint8 r, Uint8 g,Uint8 b, SDL_Renderer* renderer)
    {
       SDL_SetRenderDrawColor(renderer,r,g,b,255);  // green
       SDL_Rect dot = {x,y,size,size};
       SDL_RenderFillRect(renderer,&dot);
    }
    void draw_head (string path, SDL_Renderer* renderer)
    {
        snake_head = loadTexture(path, renderer);
        SDL_Rect dot = {x,y-5,DOT_SIZE+5,DOT_SIZE+10};
        SDL_RenderCopy(renderer,snake_head,nullptr,&dot);
    }
    void move(string mode)  // chỉ với body[0]
    {
        if (mode == "classic") {
            x+=directionX;
            y+=directionY;
            if (x == SCREEN_WIDTH-FRAME_RIGHT) x = FRAME_LEFT;       // cạnh phải
            if (x == FRAME_LEFT-size) x = SCREEN_WIDTH-FRAME_RIGHT-size;      // cạnh trái
            if (y == SCREEN_HEIGHT-FRAME_DOWN) y = FRAME_UP;                    // cạnh dưới
            if (y == FRAME_UP - size) y = SCREEN_HEIGHT-FRAME_DOWN-size;    // cạnh trên
        }
        else if (mode == "modern") {
            x+=directionX;
            y+=directionY;
        }
    }
    void  turn_right ()
    {
        directionX = step;
        directionY = 0;
    }
    void turn_left ()
    {
        directionX = -step;
        directionY = 0;
    }
    void turn_down ()
    {
        directionX = 0;
        directionY = step;
    }
    void turn_up ()
    {
        directionX = 0;
        directionY = -step;
    }
    bool inside (int minX, int minY, int maxX, int maxY)
    {
       return (minX<=x && minY<=y && x+size<=maxX && y+size<=maxY);
    }

};


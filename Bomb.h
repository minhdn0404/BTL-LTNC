
#pragma once
#include "random.h"

struct Bomb
{
    int x;
    int y;
    SDL_Texture* blue_bomb_texture;
    SDL_Texture* red_bomb_texture;
    SDL_Texture* explode_texture;
    string bomb_type [NUMBER_OF_BOMB];
    int current_index_bomb_type; // 0/1

    Bomb ()
    {
        bomb_type[0] = BLUE_BOMB_PATH;
        bomb_type[1] = RED_BOMB_PATH;
        x = 0;
        y = 0;
        random_bomb_type();
    }
    ~Bomb ()
    {
        SDL_DestroyTexture(blue_bomb_texture);
        SDL_DestroyTexture(red_bomb_texture);
        blue_bomb_texture = nullptr;
        red_bomb_texture = nullptr;
    }
    void random_generate()
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
    }

    void random_bomb_type ()
    {
        int i = random(0,9);
        current_index_bomb_type = RANDOM_BOMB_POSSIBILITY[i];
    }

    void load_image (SDL_Renderer* renderer)
    {
        blue_bomb_texture = loadTexture(bomb_type[0], renderer);
        red_bomb_texture = loadTexture(bomb_type[1], renderer);
    }
    void draw_bomb (SDL_Renderer* renderer) // 0 <= i <= 1
    {
        SDL_Rect dot = {x,y,DOT_SIZE+5,DOT_SIZE+5};
        if (current_index_bomb_type ==0) SDL_RenderCopy(renderer,blue_bomb_texture,nullptr,&dot);
        else SDL_RenderCopy(renderer,red_bomb_texture,nullptr,&dot);
    }

    bool is_eaten_by (Snake snake)
    {
       if (x==snake.body[0].x && y==snake.body[0].y) return true;
       else return false;
    }
};

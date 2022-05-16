
#pragma once

struct Bomb
{
    int x;
    int y;
    SDL_Texture* bomb_texture;
    string bomb_type [NUMBER_OF_BOMB];
    Bomb ()
    {
        bomb_type[0] = BLUE_BOMB_PATH;
        bomb_type[1] = RED_BOMB_PATH;
        x = 0;
        y = 0;
    }
    ~Bomb ()
    {
        SDL_DestroyTexture(bomb_texture);
        bomb_texture = nullptr;
    }
    void random_generate()
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
    }

    void draw_bomb (int i, SDL_Renderer* renderer) // 0 <= i <= 9
    {
        bomb_texture = loadTexture(bomb_type[RANDOM_BOMB_POSSIBILITY[i]], renderer);
        SDL_Rect dot = {x,y,DOT_SIZE+5,DOT_SIZE+5};
        SDL_RenderCopy(renderer,bomb_texture,nullptr,&dot);
    }
    bool is_eaten_by (Snake snake)
    {
       if (x==snake.body[0].x && y==snake.body[0].y) return true;
       else return false;
    }
};

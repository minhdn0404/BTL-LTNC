
#pragma once
#include <cmath>

int random (int min, int max)
{
    return min+rand()%(max-min+1);
}


struct Food
{
    int x;
    int y;
    SDL_Texture* food_texture;
    string food_type [NUMBER_OF_FOOD];

    Food ()
    {
        food_type[0] = FOOD_APPLE_PATH;
        food_type[1] = FOOD_BANANA_PATH;
        food_type[2] = FOOD_GRAPE_PATH;
        food_type[3] = FOOD_LEMON_PATH;
        food_type[4] = FOOD_ORANGE_PATH;
    }
    ~Food ()
    {
        SDL_DestroyTexture(food_texture);
        food_texture = nullptr;
    }
    void random_generate ()
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
    }
    void draw_food (int i, SDL_Renderer* renderer) // 0 <= i <= 19
    {
        food_texture = loadTexture(food_type[RANDOM_FOOD_POSSIBILITY[i]], renderer);
        SDL_Rect dot = {x,y,DOT_SIZE+4,DOT_SIZE+4};
        SDL_RenderCopy(renderer,food_texture,nullptr,&dot);
    }
    bool is_eaten_by (Snake snake)
    {
       if (x==snake.body[0].x && y==snake.body[0].y) return true;
       else return false;
    }

};

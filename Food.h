
#pragma once
#include <cmath>
#include "random.h"
#include "Bomb.h"
#include "obstacle.h"

struct Food
{
    int x;
    int y;
    SDL_Texture* food_texture;
    string food_type [NUMBER_OF_FOOD];
    int current_index_food_type;  // 0/1/2/3/4

    Food ()
    {
        food_type[0] = FOOD_APPLE_PATH;
        food_type[1] = FOOD_BANANA_PATH;
        food_type[2] = FOOD_GRAPE_PATH;
        food_type[3] = FOOD_LEMON_PATH;
        food_type[4] = FOOD_ORANGE_PATH;
        x = 0;
        y = 0;
        random_food_type();
    }
    ~Food ()
    {
        SDL_DestroyTexture(food_texture);
        food_texture = nullptr;
    }

    bool checkEmpty (int x_, int y_, Snake snake) // classic
    {
        for (int i=0; i<snake.body.size(); ++i)
        {
            if (x_ == snake.body[i].x && y_ == snake.body[i].y) return false;
        }
        return true;
    }
    bool checkEmpty (int x_, int y_, Snake snake, vector<Obstacle> obstacles) // modern
    {
        for (int i=0; i<snake.body.size(); ++i)
        {
            if (x_ == snake.body[i].x && y_ == snake.body[i].y) return false;
        }
        for (int i=0; i<obstacles.size(); ++i)
        {
            if (x_ == obstacles[i].x && y_ == obstacles[i].y) return false;
        }
        return true;
    }

    void random_generate (Snake snake)
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
        if (checkEmpty(x,y,snake)==false) {
             random_generate(snake);
        }
    }

    void random_generate(Snake snake, vector<Obstacle> obstacles)
    {
        x = DOT_SIZE*(random(6,58));
        y = DOT_SIZE*(random(8,42));
        if (checkEmpty(x,y,snake,obstacles)==false) {
            random_generate(snake,obstacles);
        }
    }
    void random_generate (Snake snake1, Snake snake2)
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
        if (checkEmpty(x,y,snake1)==false || checkEmpty(x,y,snake2)==false) {
            random_generate(snake1, snake2);
        }
    }

    void random_food_type()
    {
        int i = random(0,19);
        current_index_food_type = RANDOM_FOOD_POSSIBILITY[i];
    }
    void draw_food ( SDL_Renderer* renderer) // 0 <= i <= 19
    {
        food_texture = loadTexture(food_type[current_index_food_type], renderer);
        SDL_Rect dot = {x,y,DOT_SIZE+4,DOT_SIZE+4};
        SDL_RenderCopy(renderer,food_texture,nullptr,&dot);
    }
    bool is_eaten_by (Snake snake)
    {
       if (x==snake.body[0].x && y==snake.body[0].y) return true;
       else return false;
    }

};


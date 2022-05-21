
#pragma once
#include "random.h"

struct Obstacle
{
    int x;
    int y;
    int bsize;
    Obstacle ()
    {
        x = -1;
        y = -1;
        bsize = DOT_SIZE;
    }
    void random_generate (int row_state)
    {
        // 50 <= x <=570
        // 5 <= x0 <=57
        int x0 = random(5,57);
        x = DOT_SIZE*x0;
        if (row_state == 1) // odd
        {
            // 70 <= y <= 410
            // 7 <= y0 <=41
            int y0 = random(3,20);
            y0 = y0*2+1;
            y = DOT_SIZE*y0;
        }
        else { // even
            // 80 <= y <= 420
            // 8 <= y0 <=42
            int y0 = random(4,21);
            y0 = y0*2;
            y = DOT_SIZE*y0;
        }
    }

    bool checkEmpty (int x_, int y_,Snake snake)
    {
        for (int i=0; i<snake.body.size(); ++i)
        {
            if (x_ == snake.body[i].x && y_ == snake.body[i].y) return false;
        }
        return true;
    }

    void random_generate (int row_state,Snake snake)
    {
        random_generate(row_state);
        if (checkEmpty(x,y,snake) == false) random_generate(row_state,snake);
    }
    void draw (Uint8 r, Uint8 g,Uint8 b, SDL_Renderer* renderer)
    {
       SDL_SetRenderDrawColor(renderer,r,g,b,255);
       SDL_Rect dot = {x,y,bsize,bsize};
       SDL_RenderFillRect(renderer,&dot);
    }

    bool is_collided_by (Snake snake)
    {
        if (snake.body[0].x == x && snake.body[0].y == y) return true;
        else return false;
    }

};


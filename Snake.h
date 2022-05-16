
#pragma once
#include <vector>
#include <string>
#include "Dot.h"
using namespace std;

struct Snake
{
    vector<Dot> body ;
    int score;
    bool right;
    bool left;
    bool up;
    bool down;

    Snake ()
    {
      up = 0;
      down = 0;
      right = 0;
      left = 0;
    }
    void draw (Uint8 r,Uint8 g, Uint8 b, string path, SDL_Renderer* renderer)
    {
        for (int i=0; i<body.size(); ++i)
        {
            if (i!=0) body[i].draw(r,g,b,renderer);
            else {
                body[i].draw_head(path,renderer);
            }
        }
    }

    void move (string mode)
    {
        if (mode == "classic") {
            for (int i=0; i<body.size(); ++i)
            {
               if (i==0) {
                 body[i].x0 = body[i].x;
                 body[i].y0 = body[i].y;
                 body[i].move("classic");

               }
               else {
                 body[i].x0 = body[i].x;
                 body[i].y0 = body[i].y;
                 body[i].x = body[i-1].x0;
                 body[i].y = body[i-1].y0;
               }
            }
        }
        else if (mode == "modern") {
            for (int i=0; i<body.size(); ++i)
            {
               if (i==0) {
                  body[i].x0 = body[i].x;
                  body[i].y0 = body[i].y;
                  body[i].move("modern");

               }
               else {
                  body[i].x0 = body[i].x;
                  body[i].y0 = body[i].y;
                  body[i].x = body[i-1].x0;
                  body[i].y = body[i-1].y0;
               }
            }
        }

    }

    void turn (int direct)
    {
        for (int i=0; i<body.size(); ++i)
        {
           if (i==0) {
             body[i].x0 = body[i].x;
             body[i].y0 = body[i].y;
             switch (direct)
             {
                 case 6:
                 {
                     if (left == 0) {
                        body[i].turn_right();
                        left = 0; right = 1; up = 0; down =0;
                        break;
                     }
                     else break;
                 }
                 case 4 :
                 {
                     if (right == 0) {
                         body[i].turn_left();
                         left = 1; right = 0; up = 0; down =0;
                         break;
                     }
                     else break;
                 }
                 case 8 :
                 {
                     if (down == 0) {
                         body[i].turn_up();
                         left = 0; right = 0; up = 1; down =0;
                         break;
                     }
                     else break;
                 }
                 case 2 :
                 {
                     if (up == 0) {
                         body[i].turn_down();
                         left = 0; right = 0; up = 0; down =1;
                         break;
                     }
                     else break;
                 }
                 default: break;
             }
           }
           else {
             body[i].x0 = body[i].x;
             body[i].y0 = body[i].y;
             body[i].x = body[i-1].x0;
             body[i].y = body[i-1].y0;
           }
        }
    }

    void update ()
    {
        Dot tail;
        tail.x = body[body.size()-1].x0;
        tail.y = body[body.size()-1].y0;
        body.push_back(tail);
    }

    bool eat_itself ()
    {
        for (int i=2; i<body.size(); ++i)
        {
            if (body[0].x == body[i].x && body[0].y == body[i].y) return true;
        }
        return false;
    }
    bool eat_other (Snake &other_snake)      // for snake vs snake mode
    {
        for (int i=0; i<other_snake.body.size(); ++i)
        {
            if (body[0].x == other_snake.body[i].x && body[0].y == other_snake.body[i].y) return true;
        }
        return false;
    }
    void increase_score (int type_of_food)
    {
        if (type_of_food == 0) score+=1;
        else if (type_of_food == 1 || type_of_food == 2) score+=2;
        else if (type_of_food == 3) score+=3;
        else if (type_of_food == 4) score+=5;
    }
    void decrease_score (int type_of_bomb)
    {
        if (type_of_bomb == 0) score -= 3;
        else score -= 5;
    }
};


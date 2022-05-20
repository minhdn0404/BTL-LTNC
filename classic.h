
#pragma once
#include "timer.h"
#include "random.h"
#include <vector>

void classic ()
{
    Game new_game;
    Snake snake;
    init_game(snake,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    Food food;
    food.random_generate_classic(snake);

    int current_number_of_bombs = 0;
    Bomb bombs [20];
    for (int i=0; i<20; ++i)
    {
        bombs[i].random_generate_classic(snake);
        bombs[i].load_image(renderer);  // load cả 2 ảnh
    }

    bool eat_food_check = 0;
    bool eat_bomb_check = 0;
    while (new_game.is_Over(snake,"classic") == false)
    {

         if (snake.score <= 200) current_number_of_bombs = (snake.score)/10;
         else current_number_of_bombs = 20;

         snake.move("classic");
         int food_index;
         if (food.is_eaten_by(snake)==true) {
            snake.increase_score(food.current_index_food_type);  // tăng điểm dựa vào thức ăn
            eat_food_check = 1;
            eat_bomb_check = 0;
            food_index = food.current_index_food_type;
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            food.random_generate_classic(snake);   // random vị trí thức ăn
            food.random_food_type();  // random loại thức ăn
            for (int i=0; i<current_number_of_bombs; ++i) // random bomb
            {
                bombs[i].random_generate_classic(snake);
                bombs[i].random_bomb_type();
            }
         }

         int bomb_index;
         int x_exp;
         int y_exp;
         for (int i=0; i<current_number_of_bombs; ++i) {
            if (bombs[i].is_eaten_by(snake)) {
                snake.decrease_score(bombs[i].current_index_bomb_type);  // trừ điểm dựa vào loại bomb
                eat_bomb_check = 1;
                eat_food_check = 0;
                cout << "Score: " << snake.score << endl;
                bomb_index = bombs[i].current_index_bomb_type;
                x_exp = bombs[i].x;
                y_exp = bombs[i].y;
                for (int i=0; i<current_number_of_bombs; ++i)
                {
                  bombs[i].random_generate_classic(snake);
                  bombs[i].random_bomb_type();
                }
                break;
            }
         }

         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background(renderer);
         new_game.draw_frame(48, 92, 240,renderer);
         new_game.draw_banner(29, 243, 10, "Classic",renderer, gFont);
         new_game.draw_score(snake.score,255,0,0,SCREEN_WIDTH/2,20,200,50,renderer,gFont);
         if (eat_food_check == 1) {
            new_game.draw_updated_score(1,food_index,255,0,0,SCREEN_WIDTH - 60, 20, 50, 50, renderer, gFont);
         }
         else if (eat_bomb_check == 1) {
            new_game.explode(renderer,x_exp,y_exp);
            new_game.draw_updated_score(0,bomb_index,255,0,0,SCREEN_WIDTH - 60, 20, 50, 50, renderer, gFont);
         }
         new_game.draw_bombs_number(current_number_of_bombs,248, 117, 27,20, SCREEN_HEIGHT-40,200,50,renderer,gFont);
         snake.draw(0,255,0,"Background/green_head.png",renderer);
         food.draw_food(renderer);

         for (int i=0; i<current_number_of_bombs; ++i)
         {
             bombs[i].draw_bomb(renderer);
         }

         SDL_RenderPresent(renderer);
         SDL_Delay(DELAY_TIME);

         SDL_Event e;
         if (SDL_PollEvent(&e)==0) continue;
         else if (e.type == SDL_QUIT) break;
         else if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym)
            {
                case SDLK_RIGHT: snake.turn(6); break;
                case SDLK_LEFT: snake.turn(4); break;
                case SDLK_UP: snake.turn(8); break;
                case SDLK_DOWN: snake.turn(2); break;
                default: break;
            }
         }

    }
    cout << "Game Over";
}



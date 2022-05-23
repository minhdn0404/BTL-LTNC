
#pragma once
#include "obstacle.h"

void modern ()
{
    Game new_game;
    Snake snake;
    init_game(snake,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    Food food;
    vector<Obstacle> obstacles;
    int row_state = 1;
    food.random_generate(snake,obstacles);

    bool eat_food_check = 0;
    while (new_game.is_Over(snake,obstacles) == false)
    {
         snake.move("modern");
         int food_index;
         if (food.is_eaten_by(snake)==true) {
            eat_food_check = 1;
            snake.increase_score(food.current_index_food_type);  // tăng điểm dựa vào thức ăn
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            food_index = food.current_index_food_type;
            food.random_generate(snake,obstacles);   // random thức ăn
            food.random_food_type();

            Obstacle obs;
            obs.random_generate(row_state,snake);
            obstacles.push_back(obs);
            cout << "Size: " << obstacles.size() << endl;
         }

         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background(renderer);
         new_game.draw_frame(48, 92, 240,renderer);
         new_game.draw_banner(29, 243, 10, "Modern",renderer, gFont);
         new_game.draw_score(snake.score,255,0,0,SCREEN_WIDTH/2,20,200,50,renderer, gFont);
         if (eat_food_check = 1) {
            new_game.draw_updated_score(1,food_index,255,0,0,SCREEN_WIDTH - 60, 20, 50, 50,renderer, gFont);
         }
         snake.draw(0,255,0,"Background/green_head.png",renderer);

         for (int i=0; i<obstacles.size(); ++i)
         {
             obstacles[i].draw(255, 255, 4,renderer);
         }

         food.draw_food(renderer);
         SDL_RenderPresent(renderer);
         SDL_Delay(DELAY_TIME+10);

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
    modern_outro(snake.score,renderer);
}

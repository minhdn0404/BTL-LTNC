
#pragma once

void classic ()
{
    Game new_game;
    Snake snake;
    init_game(snake,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    snake.score = 0;
    Food food;
    food.random_generate();

    int current_number_of_bombs = 0;
    vector<Bomb> bombs;
    for (int i=0; i<5; ++i)
    {
        Bomb b;
        b.random_generate();
        bombs.push_back(b);
    }


    int index_food_path = 0; // loại đồ ăn ngẫu nhiên - apple
    int index_bomb_path = 0; // loại bomb ngẫu nhiên - blue

    while (new_game.is_Over(snake,"classic") == false)
    {
         snake.move("classic");
         bool eaten_check = 0;
         if (food.is_eaten_by(snake)==true) {
            snake.increase_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            eaten_check = 1;
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate();   // random thức ăn
            cout << "BOMBS: " << current_number_of_bombs << endl;
         }

         current_number_of_bombs = (snake.score)/10;

         bool eat_bomb_check = 0;
         for (int i=0; i<current_number_of_bombs; ++i) {
            if (bombs[i].is_eaten_by(snake)) {
                snake.decrease_score(RANDOM_BOMB_POSSIBILITY[index_bomb_path]);  // trừ điểm dựa vào loại bomb
                eat_bomb_check = 1;
                cout << "Score: " << snake.score << endl;
                index_bomb_path = random(0,9);
                for (int i=0; i<current_number_of_bombs; ++i)
                {
                  bombs[i].random_generate();
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
         new_game.draw_updated_score(RANDOM_FOOD_POSSIBILITY[index_food_path],255,0,0,SCREEN_WIDTH - 60, 20, 50, 50, renderer, gFont);
         snake.draw(0,255,0,"Background/green_head.png",renderer);
         food.draw_food(index_food_path, renderer);
         for (int i=0; i<current_number_of_bombs; ++i)
         {
             bombs[i].draw_bomb(index_bomb_path, renderer);
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


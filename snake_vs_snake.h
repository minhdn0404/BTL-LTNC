
#pragma once

void snake_vs_snake ()
{
    Game new_game;
    Snake snake1;
    Snake snake2;
    init_game(snake1,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    init_game(snake2,FRAME_LEFT+40,FRAME_UP+60,FRAME_LEFT+30,FRAME_UP+60,FRAME_LEFT+20,FRAME_UP+60,FRAME_LEFT+10,FRAME_UP+60);
    Food food;
    food.random_generate(snake1,snake2);
    int index_food_path = 0;

    int current_number_of_bombs = 0;
    Bomb bombs [5];
    for (int i=0; i<5; ++i)
    {
        bombs[i].random_generate(snake1,snake2);
        bombs[i].load_image(renderer);  // load cả 2 ảnh
    }

    bool eat_food_check = 0;
    bool eat_bomb_check = 0;
    Uint32 start_time = 0;
    while (new_game.is_Over(snake1, snake2) == false)
    {

        if (snake1.score+snake2.score <= 50) current_number_of_bombs = (snake1.score+snake2.score)/10;
        else current_number_of_bombs = 5;

        snake1.move("classic");
        snake2.move("classic");
        int food_index;
        if (food.is_eaten_by(snake1)==true ) {
            snake1.increase_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score1: " << snake1.score << endl;
            start_time = SDL_GetTicks();
            cout << "Current Time: " << start_time << endl;
            snake1.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate(snake1,snake2);   // random thức ăn
            food.random_food_type();
            for (int i=0; i<current_number_of_bombs; ++i)
            {
                bombs[i].random_generate(snake1,snake2);
                bombs[i].random_bomb_type();
            }
         }
        if (food.is_eaten_by(snake2)==true ) {
            snake2.increase_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score2: " << snake2.score << endl;
            start_time = SDL_GetTicks();
            cout << "Current Time: " << start_time << endl;
            snake2.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate(snake1,snake2);   // random thức ăn
            food.random_food_type();
            for (int i=0; i<current_number_of_bombs; ++i)
            {
                bombs[i].random_generate(snake1,snake2);
                bombs[i].random_bomb_type();
            }
         }


         int bomb_index;
         int x_exp;
         int y_exp;
         for (int i=0; i<current_number_of_bombs; ++i) {
            if (bombs[i].is_eaten_by(snake1)) {
                snake1.decrease_score(bombs[i].current_index_bomb_type);  // trừ điểm dựa vào loại bomb
                cout << "Score: " << snake1.score << endl;
                bomb_index = bombs[i].current_index_bomb_type;
                x_exp = bombs[i].x;
                y_exp = bombs[i].y;
                for (int i=0; i<current_number_of_bombs; ++i)
                {
                  bombs[i].random_generate(snake1,snake2);
                  bombs[i].random_bomb_type();
                }
                break;
            }
            if (bombs[i].is_eaten_by(snake2)) {
                snake2.decrease_score(bombs[i].current_index_bomb_type);  // trừ điểm dựa vào loại bomb
                cout << "Score: " << snake2.score << endl;
                bomb_index = bombs[i].current_index_bomb_type;
                x_exp = bombs[i].x;
                y_exp = bombs[i].y;
                for (int i=0; i<current_number_of_bombs; ++i)
                {
                  bombs[i].random_generate(snake1,snake2);
                  bombs[i].random_bomb_type();
                }
                break;
            }

         }

         if (SDL_GetTicks() - start_time > 10000) {
            start_time = SDL_GetTicks();
            index_food_path = random(0,19);
            food.random_generate(snake1,snake2);   // random thức ăn
            food.random_food_type();
            cout << "Out of time" << endl;
         }

        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderClear(renderer);
        new_game.draw_play_background(renderer);
        new_game.draw_frame(48, 92, 240,renderer);
        new_game.draw_banner(29, 243, 10, "Snake vs Snake",renderer,gFont);
        new_game.draw_score(snake1.score,255, 162, 0,SCREEN_WIDTH/2,10,200,30,renderer,gFont);
        new_game.draw_score(snake2.score,45, 212, 199,SCREEN_WIDTH/2,40,200,30,renderer,gFont);
        new_game.draw_timer(255, 0, 170,20, SCREEN_HEIGHT-40,200,50,start_time, SDL_GetTicks(),renderer,gFont);
        snake1.draw(255, 162, 0,"Background/orange_head.png",renderer);
        snake2.draw(11, 245, 228,"Background/cyan_head.png",renderer);
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
                case SDLK_RIGHT: snake1.turn(6); break;
                case SDLK_LEFT: snake1.turn(4); break;
                case SDLK_UP: snake1.turn(8); break;
                case SDLK_DOWN: snake1.turn(2); break;
                case SDLK_d: snake2.turn(6); break;
                case SDLK_a: snake2.turn(4); break;
                case SDLK_w: snake2.turn(8); break;
                case SDLK_s: snake2.turn(2); break;
                default: break;
            }
         }
    }

    if (snake1.score == MAX_SCORE || snake2.score == MAX_SCORE)
    {
        if (snake1.score > snake2.score) cout << "Snake 1 win" << endl;
        else cout << "Snake 2 win" << endl;
    }
    else {
        if (snake1.eat_other(snake2)==true) cout << "Snake 2 win" << endl;
        else if (snake2.eat_other(snake1) == true) cout << "Snake 1 win" << endl;
     }
    cout << "Game Over";
}


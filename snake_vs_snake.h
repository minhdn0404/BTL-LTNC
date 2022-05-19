#pragma once

void snake_vs_snake ()
{
    Game new_game;
    Snake snake1;
    Snake snake2;
//    snake1.load_head_image("Background/orange_head.png",renderer);
//    snake2.load_head_image("Background/cyan_head.png",renderer);
    init_game(snake1,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    init_game(snake2,FRAME_LEFT+40,FRAME_UP+60,FRAME_LEFT+30,FRAME_UP+60,FRAME_LEFT+20,FRAME_UP+60,FRAME_LEFT+10,FRAME_UP+60);
    Food food;
    food.random_generate();
    int index_food_path = 0;

    while (new_game.is_Over_svs(snake1, snake2) == false)
    {
         snake1.move("classic");
         snake2.move("classic");
         if (food.is_eaten_by(snake1)==true ) {
            snake1.increase_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score1: " << snake1.score << endl;
            snake1.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate();   // random thức ăn
            food.random_food_type();
         }
         if (food.is_eaten_by(snake2)==true ) {
            snake2.increase_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score2: " << snake2.score << endl;
            snake2.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
            food.random_food_type();
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background(renderer);
         new_game.draw_frame(48, 92, 240,renderer);
         new_game.draw_banner(29, 243, 10, "Snake vs Snake",renderer,gFont);
         new_game.draw_score(snake1.score,255, 162, 0,SCREEN_WIDTH/2,10,200,30,renderer,gFont);
         new_game.draw_score(snake2.score,45, 212, 199,SCREEN_WIDTH/2,40,200,30,renderer,gFont);
         new_game.draw_updated_score(1,RANDOM_FOOD_POSSIBILITY[index_food_path],255,0,0,SCREEN_WIDTH - 60, 20, 50, 50,renderer, gFont);
         snake1.draw(255, 162, 0,"Background/orange_head.png",renderer);
         snake2.draw(45, 212, 199,"Background/cyan_head.png",renderer);
         food.draw_food(renderer);
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


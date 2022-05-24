

#include "library.h"

using namespace std;

void mode (Button classic_button, Button modern_button, Button svs_button, string path1, string path2, string path3)
{
    SDL_Texture *texture;
    texture = loadTexture("Background/mode.jpg",renderer);
    SDL_RenderCopy(renderer,texture,nullptr,nullptr);

    classic_button.load_image(renderer, path1);
    modern_button.load_image(renderer, path2);
    svs_button.load_image(renderer, path3);
    classic_button.draw(renderer);
    modern_button.draw(renderer);
    svs_button.draw(renderer);

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    texture = nullptr;
}

int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    srand(time(0));

    intro();

    Button play_button;
    play_button.set_position(SCREEN_WIDTH/3+5,SCREEN_HEIGHT/3+60,220,100);
    play_button.load_image(renderer,"Background/over_play_button.png");
    play_button.draw(renderer);
    SDL_RenderPresent(renderer);


    bool game_is_running = true;
    bool choose_mode = false;
    bool check_classic = false;
    bool check_modern = false;
    bool check_svs = false;
    while (game_is_running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) && choose_mode == false)
        {
            if (e.type == SDL_QUIT) game_is_running = false;
            else
            {
                play_button.handle_event(e);
                if (play_button.current_state == MOUSE_OUT)
                {
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_RenderClear(renderer);
                    out_over_mouse_intro(play_button,"Background/out_play_button.png");
                }
                else if (play_button.current_state == MOUSE_OVER)
                {
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_RenderClear(renderer);
                    out_over_mouse_intro(play_button,"Background/over_play_button.png");
                }
                else if (play_button.current_state == MOUSE_DOWN)
                {
                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_RenderClear(renderer);
                    down_mouse_intro(play_button);
                }
                else if (play_button.current_state == MOUSE_UP)
                {
                    choose_mode = true;
                    break;
                }
                SDL_RenderPresent(renderer);
            }
        }

        if (choose_mode == true) {
            Button classic_button;
            Button modern_button;
            Button svs_button;
            classic_button.set_position(150,160,300,60);
            modern_button.set_position(150,240,300,60);
            svs_button.set_position(150,320,300,60);
            mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON, OUT_MODERN_BUTTON, OUT_SVS_BUTTON);

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) game_is_running = false;
                else
                {
                    classic_button.handle_event(e);
                    modern_button.handle_event(e);
                    svs_button.handle_event(e);

                    SDL_SetRenderDrawColor(renderer,255,255,255,255);
                    SDL_RenderClear(renderer);

                    if (classic_button.current_state == MOUSE_OVER)
                    {
                        classic_button.set_position(150,160,300,60);
                        mode(classic_button,modern_button,svs_button,OVER_CLASSIC_BUTTON,OUT_MODERN_BUTTON,OUT_SVS_BUTTON);
                    }
                    else if (modern_button.current_state == MOUSE_OVER)
                    {
                        modern_button.set_position(150,240,300,60);
                        mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON, OVER_MODERN_BUTTON, OUT_SVS_BUTTON);
                    }
                    else if (svs_button.current_state == MOUSE_OVER)
                    {
                        svs_button.set_position(150,320,300,60);
                        mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON, OUT_MODERN_BUTTON, OVER_SVS_BUTTON);
                    }
                    else if (classic_button.current_state == MOUSE_DOWN) {
                        classic_button.set_position(150+5,160+5,300-5,60-5);
                        mode(classic_button,modern_button,svs_button,OVER_CLASSIC_BUTTON,OUT_MODERN_BUTTON,OUT_SVS_BUTTON);
                    }
                    else if (modern_button.current_state == MOUSE_DOWN) {
                        modern_button.set_position(150+5,240+5,300-5,60-5);
                        mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON, OVER_MODERN_BUTTON, OUT_SVS_BUTTON);
                    }
                    else if (svs_button.current_state == MOUSE_DOWN) {
                        svs_button.set_position(150+5,320+5,300-5,60-5);
                        mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON, OUT_MODERN_BUTTON, OVER_SVS_BUTTON);
                    }
                    else if (classic_button.current_state == MOUSE_UP) {
                        check_classic = true;
                        break;
                    }
                    else if (modern_button.current_state == MOUSE_UP){
                        check_modern = true;
                        break;
                    }
                    else if (svs_button.current_state == MOUSE_UP) {
                        check_svs = true;
                        break;
                    }
                    else if ( classic_button.current_state == MOUSE_OUT
                           && modern_button.current_state == MOUSE_OUT
                           && svs_button.current_state == MOUSE_OUT)
                    {
                        classic_button.set_position(150,160,300,60);
                        modern_button.set_position(150,240,300,60);
                        svs_button.set_position(150,320,300,60);
                        mode(classic_button,modern_button,svs_button,OUT_CLASSIC_BUTTON,OUT_MODERN_BUTTON,OUT_SVS_BUTTON);
                    }

                }
            }
            if (check_classic == true) {
                classic();
                break;
            }
            else if (check_modern == true) {
                modern();
                break;
            }
            else if (check_svs == true) {
                snake_vs_snake();
                break;
            }
        }

   }

    waitUntilKeyPressed();
    quitSDL(window, renderer, gFont);
    return 0;
}


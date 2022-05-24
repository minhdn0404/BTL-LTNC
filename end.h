#pragma once

void draw_button (Button play_again_button, Button menu_button,Button quit_button, string path1, string path2, string path3)
{
    play_again_button.load_image(renderer, path1);
    menu_button.load_image(renderer, path2);
    quit_button.load_image(renderer, path3);
    play_again_button.draw(renderer);
    menu_button.draw(renderer);
    quit_button.draw(renderer);
}

void end (int check_mode, bool &play_again, bool &menu, bool &quit)
{
    SDL_Texture* m_texture;
    m_texture = loadTexture("Background/intro.jpg",renderer);
    SDL_RenderCopy(renderer,m_texture,nullptr,nullptr);

    Button play_again_button;
    Button menu_button;
    Button quit_button;
    play_again_button.set_position (180,80,300,100);
    menu_button.set_position(180,190,300,100);
    quit_button.set_position(180,300,300,100);

    draw_button(play_again_button, menu_button, quit_button,
                OUT_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OUT_QUIT_BUTTON);

    SDL_RenderPresent(renderer);

    bool end_wait = true;
    while (end_wait)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) end_wait = false;
            else {
                play_again_button.handle_event(e);
                menu_button.handle_event(e);
                quit_button.handle_event(e);
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer,m_texture,nullptr,nullptr);

                if (play_again_button.current_state == MOUSE_OVER) {
                    play_again_button.set_position (180,80,300,100);
                    draw_button(play_again_button, menu_button, quit_button,
                               OVER_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OUT_QUIT_BUTTON);
                }
                else if (menu_button.current_state == MOUSE_OVER) {
                    menu_button.set_position (180,190,300,100);
                    draw_button(play_again_button, menu_button, quit_button,
                               OUT_PLAY_AGAIN_BUTTON, OVER_MENU_BUTTON, OUT_QUIT_BUTTON);
                }
                else if (quit_button.current_state == MOUSE_OVER) {
                    quit_button.set_position (180,300,300,100);
                    draw_button(play_again_button, menu_button, quit_button,
                               OUT_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OVER_QUIT_BUTTON);
                }
                else if (play_again_button.current_state == MOUSE_DOWN) {
                    play_again_button.set_position (180+10,80+10,300-10,100-10);
                    draw_button(play_again_button, menu_button, quit_button,
                               OVER_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OUT_QUIT_BUTTON);
                }
                else if (menu_button.current_state == MOUSE_DOWN) {
                    menu_button.set_position (180+10,190+10,300-10,100-10);
                    draw_button(play_again_button, menu_button, quit_button,
                               OUT_PLAY_AGAIN_BUTTON, OVER_MENU_BUTTON, OUT_QUIT_BUTTON);
                }
                else if (quit_button.current_state == MOUSE_DOWN) {
                    quit_button.set_position (180+10,300+10,300-10,100-10);
                    draw_button(play_again_button, menu_button, quit_button,
                               OUT_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OVER_QUIT_BUTTON);
                }
                else if (play_again_button.current_state == MOUSE_UP) {
                   play_again = true;
                   end_wait = false;
                }
                else if (menu_button.current_state == MOUSE_UP) {
                    menu = true;
                    end_wait = false;
                }
                else if (quit_button.current_state == MOUSE_UP)
                {
                    quit = true;
                    end_wait = false;
                }
                else if (play_again_button.current_state == MOUSE_OUT && menu_button.current_state == MOUSE_OUT
                    && quit_button.current_state == MOUSE_OUT)
                {
                    play_again_button.set_position (180,80,300,100);
                    menu_button.set_position(180,190,300,100);
                    quit_button.set_position(180,300,300,100);
                    draw_button(play_again_button, menu_button, quit_button,
                               OUT_PLAY_AGAIN_BUTTON, OUT_MENU_BUTTON, OUT_QUIT_BUTTON);
                }
                SDL_RenderPresent(renderer);

            }
        }
    }

    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}

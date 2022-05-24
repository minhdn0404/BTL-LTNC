#pragma pnce


void next()
{
    Button next_button;
    next_button.set_position(430,130,100,60);
    next_button.load_image(renderer,OUT_NEXT_BUTTON);
    next_button.draw(renderer);
    SDL_RenderPresent(renderer);

    bool next_wait = true;
    while (next_wait)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) next_wait = false;
            else {
                next_button.handle_event(e);
                if (next_button.current_state == MOUSE_OUT) {
                    next_button.set_position(430,130,100,60);
                    next_button.load_image(renderer,OUT_NEXT_BUTTON);
                    next_button.draw(renderer);
                    SDL_RenderPresent(renderer);
                }
                else if (next_button.current_state == MOUSE_OVER) {
                    next_button.set_position(430,130,100,60);
                    next_button.load_image(renderer,OVER_NEXT_BUTTON);
                    next_button.draw(renderer);
                    SDL_RenderPresent(renderer);
                }
                else if (next_button.current_state == MOUSE_DOWN) {
                    next_button.set_position(440,140,90,50);
                    next_button.load_image(renderer,OVER_NEXT_BUTTON);
                    next_button.draw(renderer);
                    SDL_RenderPresent(renderer);
                }
                else if (next_button.current_state == MOUSE_UP) {
                    next_wait = false;
                }
            }
        }
    }
}

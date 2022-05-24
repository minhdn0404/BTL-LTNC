
#pragma once

void intro ()
{
    SDL_Texture* intro_background = loadTexture("Background/intro.jpg",renderer);
    gFont = TTF_OpenFont( "Font/race1.ttf", 28 );
    SDL_Color textColor = {0, 200, 0};
    SDL_Texture* title = loadFromRenderedText("SNAKE",textColor, renderer, gFont);
    SDL_Rect rect = {SCREEN_WIDTH/3-30,0,300,100};

    int i = 10;
    while (rect.y<=SCREEN_HEIGHT/3-40)
    {
        rect.y +=i;
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,intro_background,nullptr,nullptr);
        SDL_RenderCopy(renderer,title,nullptr,&rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }
    SDL_DestroyTexture(intro_background);
    intro_background = nullptr;
    SDL_DestroyTexture(title);
    title = nullptr;
}

void out_over_mouse_intro (Button play_button, string path)
{
    SDL_Texture* intro_background = loadTexture("Background/intro.jpg",renderer);
    gFont = TTF_OpenFont( "Font/race1.ttf", 28 );
    SDL_Color textColor = {0, 200, 0};
    SDL_Texture* title = loadFromRenderedText("SNAKE",textColor, renderer, gFont);
    SDL_Rect rect = {183,130,300,100};
    SDL_RenderCopy(renderer,intro_background,nullptr,nullptr);
    SDL_RenderCopy(renderer,title,nullptr,&rect);
    play_button.set_position(SCREEN_WIDTH/3+5,SCREEN_HEIGHT/3+60,220,100);
    play_button.load_image(renderer,path);
    play_button.draw(renderer);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(intro_background);
    intro_background = nullptr;
    SDL_DestroyTexture(title);
    title = nullptr;
}

void down_mouse_intro (Button play_button)
{
    SDL_Texture* intro_background = loadTexture("Background/intro.jpg",renderer);
    gFont = TTF_OpenFont( "Font/race1.ttf", 28 );
    SDL_Color textColor = {0, 200, 0};
    SDL_Texture* title = loadFromRenderedText("SNAKE",textColor, renderer, gFont);
    SDL_Rect rect = {183,130,300,100};
    SDL_RenderCopy(renderer,intro_background,nullptr,nullptr);
    SDL_RenderCopy(renderer,title,nullptr,&rect);
    play_button.set_position((SCREEN_WIDTH/3+5)+10,SCREEN_HEIGHT/3+60+10,220-10,100-10);

    play_button.load_image(renderer,"Background/over_play_button.png");
    play_button.draw(renderer);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(intro_background);
    intro_background = nullptr;
    SDL_DestroyTexture(title);
    title = nullptr;
}



#pragma once

void classic_outro (int score, SDL_Renderer* renderer)
{
    SDL_Texture* texture;
    texture = loadTexture("Background/game_over.png",renderer);
    int x0 = 60;
    int y0 = 60;
    SDL_Rect rect = {50+x0,70+y0,540-x0-x0,360-y0-y0};
    SDL_RenderCopy(renderer,texture,nullptr,&rect);


    gFont = TTF_OpenFont( "Font/standard.ttf", 28 );
    SDL_Color textColor = {200, 0, 0};
    string s = "Score: " + to_string(score);
    SDL_Texture* score_text = loadFromRenderedText(s,textColor, renderer, gFont);
    SDL_Rect rect1 = {50+x0+10, 70+y0+100,350,100};
    SDL_RenderCopy(renderer,score_text,nullptr, &rect1);

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void modern_outro (int score, SDL_Renderer* renderer)
{
    SDL_Texture* texture;
    texture = loadTexture("Background/game_over.png",renderer);
    int x0 = 60;
    int y0 = 60;
    SDL_Rect rect = {50+x0,70+y0,540-x0-x0,360-y0-y0};
    SDL_RenderCopy(renderer,texture,nullptr,&rect);

    gFont = TTF_OpenFont( "Font/standard.ttf", 28 );
    SDL_Color textColor = {200, 0, 0};
    string s = "Score: " + to_string(score);
    SDL_Texture* score_text = loadFromRenderedText(s,textColor, renderer, gFont);
    SDL_Rect rect1 = {50+x0+10, 70+y0+100,350,100};
    SDL_RenderCopy(renderer,score_text,nullptr, &rect1);

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void snake_vs_snake_outro (Snake win_snake, int i)
{
    SDL_Texture* texture;
    texture = loadTexture("Background/game_over.png",renderer);
    int x0 = 60;
    int y0 = 60;
    SDL_Rect rect = {50+x0,70+y0,540-x0-x0,360-y0-y0};
    SDL_RenderCopy(renderer,texture,nullptr,&rect);

    gFont = TTF_OpenFont( "Font/standard.ttf", 28 );
    SDL_Color textColor;
    if (i==1) textColor = {255, 162, 0};
    else textColor = {11, 245, 228};

    string s = "Score: " + to_string(win_snake.score);
    string win;
    if (i == 1) win = "Snake 1 win" ;
    else win = "Snake 2 win";
    SDL_Texture* score_text = loadFromRenderedText(s,textColor, renderer, gFont);
    SDL_Rect rect1 = {50+x0+10, 70+y0+200,300,50};
    SDL_RenderCopy(renderer,score_text,nullptr, &rect1);

    score_text = loadFromRenderedText(win,textColor,renderer,gFont);
    SDL_Rect rect2 = {50+x0+20, 70+y0+50,300,50};
    SDL_RenderCopy(renderer,score_text,nullptr,&rect2);

    if (i==1) score_text = loadTexture("Background/snake1_win.png",renderer);
    else score_text = loadTexture("Background/snake2_win.png", renderer);
    SDL_Rect rect3 = {50+x0,70+y0+100,540-x0-x0,100};
    SDL_RenderCopy(renderer, score_text, nullptr, &rect3);

    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
    texture = nullptr;
}


#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Snake.h"

using namespace std;
struct Game
{
    int high_score;
    SDL_Texture* play_background;
    SDL_Texture* texture_font;

    Game()
    {
        play_background = nullptr;
        texture_font = nullptr;
    }
    ~Game()
    {
         SDL_DestroyTexture(play_background);
         play_background = nullptr;
         SDL_DestroyTexture(texture_font);
         texture_font = nullptr;
    }

    void draw_play_background (SDL_Renderer* renderer)
    {
        if (play_background==nullptr) play_background = loadTexture("Background/dark.jpg", renderer);
        SDL_RenderCopy(renderer,play_background,nullptr,nullptr);
    }

    void draw_score(const int score, Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h, SDL_Renderer* renderer, TTF_Font *gFont )
    {
        string s = "Score: " + to_string(score);
        gFont = TTF_OpenFont( "Font/zxzxzx.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(s, textColor, renderer,gFont);
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_updated_score ( int food_type, Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h, SDL_Renderer* renderer, TTF_Font *gFont )
    {
        string s;
        if (food_type == 0) s = "+ 1";
        else if (food_type == 1 || food_type == 2) s = "+ 2";
        else if (food_type == 3) s = "+ 3";
        else if (food_type == 4) s = "+ 5";
        gFont = TTF_OpenFont( "Font/zxzxzx.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(s, textColor, renderer, gFont);
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_banner (Uint8 r, Uint8 g, Uint8 b, string title, SDL_Renderer* renderer, TTF_Font *gFont)
    {
        gFont = TTF_OpenFont( "Font/pacfont.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(title, textColor, renderer, gFont);
        SDL_Rect rect = {SCREEN_WIDTH/15,10,250,50};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_frame (Uint8 r, Uint8 g, Uint8 b, SDL_Renderer* renderer)
    {
      SDL_Rect frame_up = {0,0,SCREEN_WIDTH,FRAME_UP};
      SDL_Rect frame_left = {0,0,FRAME_LEFT,SCREEN_HEIGHT};
      SDL_Rect frame_down = {0,SCREEN_HEIGHT-FRAME_DOWN,SCREEN_WIDTH,FRAME_DOWN};
      SDL_Rect frame_right = {SCREEN_WIDTH-FRAME_RIGHT,0,FRAME_RIGHT,SCREEN_HEIGHT};
      SDL_SetRenderDrawColor(renderer,r, g, b, 255);
      SDL_RenderFillRect(renderer,&frame_up);
      SDL_RenderFillRect(renderer,&frame_down);
      SDL_RenderFillRect(renderer,&frame_left);
      SDL_RenderFillRect(renderer,&frame_right);
    }

    bool is_Over (Snake &snake, string mode)
    {
       if (mode == "classic") {
           if ( (snake.eat_itself() == true) ) return true;
           else return false;
       }
       else if (mode == "modern") {
           if ( (snake.body[0].inside(FRAME_LEFT,FRAME_UP,SCREEN_WIDTH-FRAME_RIGHT,SCREEN_HEIGHT-FRAME_DOWN)==false)
             || (snake.eat_itself() == true) ) return true;
           else return false;
       }
    }
    bool is_Over_svs (Snake &snake1, Snake &snake2)
    {
       if (snake1.eat_other(snake2)==true || snake2.eat_other(snake1)==true
           || snake1.score == MAX_SCORE || snake2.score == MAX_SCORE) return true;
       else return false;
    }
};
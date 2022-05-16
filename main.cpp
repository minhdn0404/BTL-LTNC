

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <ctime>
#include "tools.h"
#include "other.h"
#include "SDL_util.h"
#include "Dot.h"
#include "Snake.h"
#include "Food.h"
#include "Game.h"
#include "init.h"
#include "classic.h"
#include "modern.h"
#include "snake_vs_snake.h"

using namespace std;

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

    intro_background = loadTexture("Background/play_button.png", renderer); // 246, 249, 11 (RGB)
    SDL_Rect rect1 = {SCREEN_WIDTH/3+5,SCREEN_HEIGHT/3+60,220,100};
    SDL_RenderCopy(renderer,intro_background,nullptr,&rect1);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(intro_background);
    intro_background = nullptr;
    SDL_DestroyTexture(title);
    title = nullptr;
}

int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    srand(time(0));

    intro();

    cout << "Choose mode: " << endl;
    cout << "1: Classic" << endl;
    cout << "2: Modern" << endl;
    cout << "3: Snake vs Snake" << endl;

    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e)==0) continue;
        else if (e.type == SDL_QUIT) break;
        else if (e.type == SDL_KEYDOWN) {
          switch (e.key.keysym.sym)
          {
              case SDLK_1: classic(); break;
              case SDLK_2: modern(); break;
              case SDLK_3: snake_vs_snake(); break;
              default: break;
          }
          break;
        }
    }


    waitUntilKeyPressed();
    quitSDL(window, renderer, gFont);
    return 0;
}






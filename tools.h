
#ifndef TOOLS_H
#define TOOLS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 100;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;
const string WINDOW_TITLE = "Snake game";
const int STEP = 10;
const int DOT_SIZE = 10;
const int DELAY_TIME = 30;
const int FRAME_UP = 70;
const int FRAME_DOWN = 50;
const int FRAME_LEFT = 50;
const int FRAME_RIGHT = 50;   // Màn hình thực: 540x360 từ (50,70) đến (590,430)
const int MAX_SCORE = 60;   // for snake vs snake mode
const int NUMBER_OF_FOOD = 5;
const int NUMBER_OF_BOMB = 2;
const string FOOD_APPLE_PATH = "Background/food_apple.png";
const string FOOD_BANANA_PATH = "Background/food_banana.png";
const string FOOD_GRAPE_PATH = "Background/food_grape.png";
const string FOOD_LEMON_PATH = "Background/food_lemon.png";
const string FOOD_ORANGE_PATH = "Background/food_orange.png";
const string BLUE_BOMB_PATH = "Background/blue_bomb.png";
const string RED_BOMB_PATH = "Background/red_bomb.png";

const int RANDOM_FOOD_POSSIBILITY [20] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,4};
/* 40% apple  +1
   20% banana +2
   20% grape  +2
   15% lemon  +3
   5% orange  +5  */

const int RANDOM_BOMB_POSSIBILITY [10] = {0,0,0,0,0,0,0,1,1,1};
/* 70% black bomb -3
   30% red bomb   -5
*/

const int MOUSE_OUT = 0;
const int MOUSE_OVER = 1;
const int MOUSE_DOWN = 2;
const int MOUSE_UP = 3;
const int TOTAL_MOUSE_STATE = 4;

const string OUT_PLAY_BUTTON = "Background/out_play_button.png"; // 246, 249, 11 (RGB)
const string OVER_PLAY_BUTTON = "Background/over_play_button.png"; // 209 192 35 (RGB)
const string OUT_CLASSIC_BUTTON = "Background/classic_button.png";
const string OVER_CLASSIC_BUTTON = "Background/over_classic_button.png";
const string OUT_MODERN_BUTTON = "Background/modern_button.png";
const string OVER_MODERN_BUTTON = "Background/over_modern_button.png";
const string OUT_SVS_BUTTON = "Background/svs_button.png";
const string OVER_SVS_BUTTON = "Background/over_svs_button.png";
const string OUT_NEXT_BUTTON = "Background/out_next_button.png";
const string OVER_NEXT_BUTTON = "Background/over_next_button.png";
const string OUT_PLAY_AGAIN_BUTTON = "Background/out_play_again_button.png";
const string OVER_PLAY_AGAIN_BUTTON = "Background/over_play_again_button.png";
const string OUT_QUIT_BUTTON = "Background/out_quit_button.png";
const string OVER_QUIT_BUTTON = "Background/over_quit_button.png";
const string OUT_MENU_BUTTON = "Background/out_menu_button.png";
const string OVER_MENU_BUTTON = "Background/over_menu_button.png";

#endif // TOOLS_H



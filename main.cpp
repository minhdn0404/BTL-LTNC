

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const string WINDOW_TITLE = "Snake game";
const int STEP = 10;
const int DOT_SIZE = 10;
const int DELAY_TIME = 50;
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
const string BLACK_BOMB_PATH = "Background/black_bomb.png";
const string RED_BOMB_PATH = "Background/red_bomb.png";

const int RANDOM_FOOD_POSSIBILITY [20] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,4};
/* 40% apple  +1
   20% banana +2
   20% grape  +2
   15% lemon  +3
   5% orange  +5  */

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadFromRenderedText( string textureText, SDL_Color textColor);
SDL_Texture* loadTexture( string path );

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font *gFont = nullptr;

struct Dot
{
    int x;
    int y;
    int x0;
    int y0;
    int size = DOT_SIZE; // kích thước dot
    int step = STEP;
    int directionX = step;
    int directionY = 0;
    SDL_Texture* snake_head;
    Dot ()
    {
        snake_head = nullptr;
    }
    ~Dot()
    {
        SDL_DestroyTexture(snake_head);
        snake_head = nullptr;
    }
    Dot(int _x, int _y)
    {
        x = _x;
        y = _y;
        snake_head = nullptr;
    }
    void draw (Uint8 r, Uint8 g,Uint8 b)
    {
       SDL_SetRenderDrawColor(renderer,r,g,b,255);  // green
       SDL_Rect dot = {x,y,size,size};
       SDL_RenderFillRect(renderer,&dot);
    }
    void draw_head (string path)
    {
        snake_head = loadTexture(path);
        SDL_Rect dot = {x,y-5,DOT_SIZE+5,DOT_SIZE+10};
        SDL_RenderCopy(renderer,snake_head,nullptr,&dot);
    }
    void move(string mode)  // chỉ với body[0]
    {
        if (mode == "classic") {
            x+=directionX;
            y+=directionY;
            if (x == SCREEN_WIDTH-FRAME_RIGHT) x = FRAME_LEFT;       // cạnh phải
            if (x == FRAME_LEFT-size) x = SCREEN_WIDTH-FRAME_RIGHT-size;      // cạnh trái
            if (y == SCREEN_HEIGHT-FRAME_DOWN) y = FRAME_UP;                    // cạnh dưới
            if (y == FRAME_UP - size) y = SCREEN_HEIGHT-FRAME_DOWN-size;    // cạnh trên
        }
        else if (mode == "modern") {
            x+=directionX;
            y+=directionY;
        }
    }
    void  turn_right ()
    {
        directionX = step;
        directionY = 0;
    }
    void turn_left ()
    {
        directionX = -step;
        directionY = 0;
    }
    void turn_down ()
    {
        directionX = 0;
        directionY = step;
    }
    void turn_up ()
    {
        directionX = 0;
        directionY = -step;
    }
    bool inside (int minX, int minY, int maxX, int maxY)
    {
       return (minX<=x && minY<=y && x+size<=maxX && y+size<=maxY);
    }

};

struct Snake
{
    vector<Dot> body ;
    int score;
    bool right;
    bool left;
    bool up;
    bool down;

    Snake ()
    {
      up = 0;
      down = 0;
      right = 0;
      left = 0;
    }
    void draw (Uint8 r,Uint8 g, Uint8 b, string path)
    {
        for (int i=0; i<body.size(); ++i)
        {
            if (i!=0) body[i].draw(r,g,b);
            else {
                body[i].draw_head(path);
            }
        }
    }

    void move (string mode)
    {
        if (mode == "classic") {
            for (int i=0; i<body.size(); ++i)
            {
               if (i==0) {
                 body[i].x0 = body[i].x;
                 body[i].y0 = body[i].y;
                 body[i].move("classic");

               }
               else {
                 body[i].x0 = body[i].x;
                 body[i].y0 = body[i].y;
                 body[i].x = body[i-1].x0;
                 body[i].y = body[i-1].y0;
               }
            }
        }
        else if (mode == "modern") {
            for (int i=0; i<body.size(); ++i)
            {
               if (i==0) {
                  body[i].x0 = body[i].x;
                  body[i].y0 = body[i].y;
                  body[i].move("modern");

               }
               else {
                  body[i].x0 = body[i].x;
                  body[i].y0 = body[i].y;
                  body[i].x = body[i-1].x0;
                  body[i].y = body[i-1].y0;
               }
            }
        }

    }

    void turn (int direct)
    {
        for (int i=0; i<body.size(); ++i)
        {
           if (i==0) {
             body[i].x0 = body[i].x;
             body[i].y0 = body[i].y;
             switch (direct)
             {
                 case 6:
                 {
                     if (left == 0) {
                        body[i].turn_right();
                        left = 0; right = 1; up = 0; down =0;
                        break;
                     }
                     else break;
                 }
                 case 4 :
                 {
                     if (right == 0) {
                         body[i].turn_left();
                         left = 1; right = 0; up = 0; down =0;
                         break;
                     }
                     else break;
                 }
                 case 8 :
                 {
                     if (down == 0) {
                         body[i].turn_up();
                         left = 0; right = 0; up = 1; down =0;
                         break;
                     }
                     else break;
                 }
                 case 2 :
                 {
                     if (up == 0) {
                         body[i].turn_down();
                         left = 0; right = 0; up = 0; down =1;
                         break;
                     }
                     else break;
                 }
                 default: break;
             }
           }
           else {
             body[i].x0 = body[i].x;
             body[i].y0 = body[i].y;
             body[i].x = body[i-1].x0;
             body[i].y = body[i-1].y0;
           }
        }
    }

    void update ()
    {
        Dot tail;
        tail.x = body[body.size()-1].x0;
        tail.y = body[body.size()-1].y0;
        body.push_back(tail);
    }

    bool eat_itself ()
    {
        for (int i=2; i<body.size(); ++i)
        {
            if (body[0].x == body[i].x && body[0].y == body[i].y) return true;
        }
        return false;
    }
    bool eat_other (Snake &other_snake)      // for snake vs snake mode
    {
        for (int i=0; i<other_snake.body.size(); ++i)
        {
            if (body[0].x == other_snake.body[i].x && body[0].y == other_snake.body[i].y) return true;
        }
        return false;
    }
    void update_score (int type_of_food)
    {
        if (type_of_food == 0) score+=1;
        else if (type_of_food == 1 || type_of_food == 2) score+=2;
        else if (type_of_food == 3) score+=3;
        else if (type_of_food == 4) score+=5;
    }

};

int random (int min, int max)
{
    return min+rand()%(max-min+1);
}

struct Food
{
    int x;
    int y;
    SDL_Texture* food_texture;
    string food_type [NUMBER_OF_FOOD];

    Food ()
    {
        food_type[0] = FOOD_APPLE_PATH;
        food_type[1] = FOOD_BANANA_PATH;
        food_type[2] = FOOD_GRAPE_PATH;
        food_type[3] = FOOD_LEMON_PATH;
        food_type[4] = FOOD_ORANGE_PATH;
    }
    ~Food ()
    {
        SDL_DestroyTexture(food_texture);
        food_texture = nullptr;
    }
    void random_generate ()
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
    }
    void draw_food (int i) // 0 <= i <= 19
    {
        food_texture = loadTexture(food_type[RANDOM_FOOD_POSSIBILITY[i]]);
        SDL_Rect dot = {x,y,DOT_SIZE+4,DOT_SIZE+4};
        SDL_RenderCopy(renderer,food_texture,nullptr,&dot);
    }
    bool is_eaten_by (Snake snake)
    {
       if (x==snake.body[0].x && y==snake.body[0].y) return true;
       else return false;
    }

};

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

    void draw_play_background ()
    {
        if (play_background==nullptr) play_background = loadTexture("Background/dark.jpg");
        SDL_RenderCopy(renderer,play_background,nullptr,nullptr);
    }

    void draw_score(const int score, Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h )
    {
        string s = "Score: " + to_string(score);
        gFont = TTF_OpenFont( "Font/zxzxzx.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(s, textColor);
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_updated_score ( int food_type, Uint8 r, Uint8 g, Uint8 b, int x, int y, int w, int h )
    {
        string s;
        if (food_type == 0) s = "+ 1";
        else if (food_type == 1 || food_type == 2) s = "+ 2";
        else if (food_type == 3) s = "+ 3";
        else if (food_type == 4) s = "+ 5";
        gFont = TTF_OpenFont( "Font/zxzxzx.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(s, textColor);
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_banner (Uint8 r, Uint8 g, Uint8 b, string title)
    {
        gFont = TTF_OpenFont( "Font/pacfont.ttf", 28 );
        SDL_Color textColor = {r, g, b};
        texture_font = loadFromRenderedText(title, textColor);
        SDL_Rect rect = {SCREEN_WIDTH/15,10,250,50};
        SDL_RenderCopy(renderer,texture_font,nullptr,&rect);
    }

    void draw_frame (Uint8 r, Uint8 g, Uint8 b)
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

void init_game (Snake &snake,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
    Dot d0(x1,y1);   // head
    Dot d1(x2,y2);
    Dot d2(x3,y3);
    Dot d3(x4,y4);
    snake.body.push_back(d0);
    snake.body.push_back(d1);
    snake.body.push_back(d2);
    snake.body.push_back(d3);
}

void classic ()
{
    Game new_game;
    Snake snake;
    init_game(snake,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    snake.score = 0;
    Food food;
    food.random_generate();
    int index_food_path = 0; // loại đồ ăn ngẫu nhiên

    while (new_game.is_Over(snake,"classic") == false)
    {
         snake.move("classic");
         bool eaten_check = 0;
         if (food.is_eaten_by(snake)==true) {
            snake.update_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            eaten_check = 1;
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background();
         new_game.draw_frame(48, 92, 240);
         new_game.draw_banner(29, 243, 10, "Classic");
         new_game.draw_score(snake.score,255,0,0,SCREEN_WIDTH/2,20,200,50);
         new_game.draw_updated_score(RANDOM_FOOD_POSSIBILITY[index_food_path],255,0,0,SCREEN_WIDTH - 60, 20, 50, 50);
         snake.draw(0,255,0,"Background/green_head.png");
         food.draw_food(index_food_path);
         SDL_RenderPresent(renderer);
         SDL_Delay(DELAY_TIME);

         SDL_Event e;
         if (SDL_PollEvent(&e)==0) continue;
         else if (e.type == SDL_QUIT) break;
         else if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym)
            {
                case SDLK_RIGHT: snake.turn(6); break;
                case SDLK_LEFT: snake.turn(4); break;
                case SDLK_UP: snake.turn(8); break;
                case SDLK_DOWN: snake.turn(2); break;
                default: break;
            }
         }

    }
    cout << "Game Over";
}

void modern ()
{
    Game new_game;
    Snake snake;
    init_game(snake,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    snake.score = 0;
    Food food;
    food.random_generate();
    int index_food_path = 0;

    while (new_game.is_Over(snake,"modern") == false)
    {
         snake.move("modern");
         if (food.is_eaten_by(snake)==true) {
            snake.update_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background();
         new_game.draw_frame(48, 92, 240);
         new_game.draw_banner(29, 243, 10, "Modern");
         new_game.draw_score(snake.score,255,0,0,SCREEN_WIDTH/2,20,200,50);
         new_game.draw_updated_score(RANDOM_FOOD_POSSIBILITY[index_food_path],255,0,0,SCREEN_WIDTH - 60, 20, 50, 50);
         snake.draw(0,255,0,"Background/green_head.png");
         food.draw_food(index_food_path);
         SDL_RenderPresent(renderer);
         SDL_Delay(DELAY_TIME);

         SDL_Event e;
         if (SDL_PollEvent(&e)==0) continue;
         else if (e.type == SDL_QUIT) break;
         else if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym)
            {
                case SDLK_RIGHT: snake.turn(6); break;
                case SDLK_LEFT: snake.turn(4); break;
                case SDLK_UP: snake.turn(8); break;
                case SDLK_DOWN: snake.turn(2); break;
                default: break;
            }
         }
    }
    cout << "Game Over";
}

void snake_vs_snake ()
{
    Game new_game;
    Snake snake1;
    Snake snake2;
    init_game(snake1,FRAME_LEFT+40,FRAME_UP+10,FRAME_LEFT+30,FRAME_UP+10,FRAME_LEFT+20,FRAME_UP+10,FRAME_LEFT+10,FRAME_UP+10);
    init_game(snake2,FRAME_LEFT+40,FRAME_UP+60,FRAME_LEFT+30,FRAME_UP+60,FRAME_LEFT+20,FRAME_UP+60,FRAME_LEFT+10,FRAME_UP+60);
    snake1.score = 0;
    snake2.score = 0;
    Food food;
    food.random_generate();
    int index_food_path = 0;

    while (new_game.is_Over_svs(snake1, snake2) == false)
    {
         snake1.move("classic");
         snake2.move("classic");
         if (food.is_eaten_by(snake1)==true ) {
            snake1.update_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score1: " << snake1.score << endl;
            snake1.update();  // tăng kích thước rắn
            index_food_path = random(0,19);
            food.random_generate();   // random thức ăn
         }
         if (food.is_eaten_by(snake2)==true ) {
            snake2.update_score(RANDOM_FOOD_POSSIBILITY[index_food_path]);  // tăng điểm dựa vào thức ăn
            cout << "Score2: " << snake2.score << endl;
            snake2.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_play_background();
         new_game.draw_frame(48, 92, 240);
         new_game.draw_banner(29, 243, 10, "Snake vs Snake");
         new_game.draw_score(snake1.score,255, 162, 0,SCREEN_WIDTH/2,10,200,30);
         new_game.draw_score(snake2.score,45, 212, 199,SCREEN_WIDTH/2,40,200,30);
         new_game.draw_updated_score(RANDOM_FOOD_POSSIBILITY[index_food_path],255,0,0,SCREEN_WIDTH - 60, 20, 50, 50);
         snake1.draw(255, 162, 0, "Background/orange_head.png");
         snake2.draw(45, 212, 199, "Background/cyan_head.png");
         food.draw_food(index_food_path);
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

void intro ()
{
    SDL_Texture* intro_background = loadTexture("Background/intro.jpg");
    gFont = TTF_OpenFont( "Font/race1.ttf", 28 );
    SDL_Color textColor = {0, 200, 0};
    SDL_Texture* title = loadFromRenderedText("SNAKE",textColor);
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

    intro_background = loadTexture("Background/play_button.png"); // 246, 249, 11 (RGB)
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
    quitSDL(window, renderer);
    return 0;
}

SDL_Texture* loadTexture( string path )
{
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == nullptr )
        cout << "Unable to load image " << path << " SDL_image Error: "
             << IMG_GetError() << endl;
    else {
        SDL_SetColorKey (loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface -> format,255,255,255 ));

        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr )
            cout << "Unable to create texture from " << path << " SDL Error: "
                 << SDL_GetError() << endl;
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

SDL_Texture* loadFromRenderedText( string textureText, SDL_Color textColor)
{
    SDL_Texture* mTexture = nullptr;
    SDL_Surface* textSurface = TTF_RenderText_Solid (gFont, textureText.c_str(), textColor);
    if (textSurface == nullptr) cout << "Unable to create text surface! " << endl << TTF_GetError();
    else {
        // Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
        if (mTexture == nullptr) cout << "Unable to create texture from rendered text!" << endl << SDL_GetError();
    }
    return mTexture;
}

void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // khi dùng máy ảo (thực hành ở trường)
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       //SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi chạy trong môi trường bình thường (không chạy trong máy ảo)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy ảo (ví dụ tại máy tính trong phòng thực hành ở trường)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    else
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor(renderer, 255,255,255,255 );

         //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
            cout <<  "SDL_image could not initialize! SDL_image Error: " << endl << IMG_GetError();
        //Initialize SDL_ttf
        if( TTF_Init() == -1 )
            cout << "SDL_ttf could not initialize! SDL_ttf Error: \n" <<  TTF_GetError() ;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    TTF_CloseFont (gFont);
    gFont = nullptr;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}





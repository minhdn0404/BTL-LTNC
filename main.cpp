



#include <iostream>
#include <SDL.h>
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
const int FRAME_UP = 40;
const int FRAME_DOWN = 20;
const int FRAME_LEFT = 20;
const int FRAME_RIGHT = 20;
const int MAX_SCORE = 60;   // for snake vs snake mode

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();

SDL_Window* window;
SDL_Renderer* renderer;

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
    Dot () {};
    Dot(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    void draw (int r, int g, int b)
    {
       SDL_SetRenderDrawColor(renderer,r,g,b,255);  // green
       SDL_Rect dot = {x,y,size,size};
       SDL_RenderFillRect(renderer,&dot);
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

    void draw (int r,int g, int b)
    {
        for (int i=0; i<body.size(); ++i)
        {
            body[i].draw(r,g,b);
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

};

int random (int min, int max)
{
    return min+rand()%(max-min+1);
}

struct Food
{
    int x;
    int y;
    void random_generate ()
    {
        x = DOT_SIZE*(random(FRAME_LEFT/DOT_SIZE, (SCREEN_WIDTH-FRAME_RIGHT-DOT_SIZE)/DOT_SIZE));
        y = DOT_SIZE*(random(FRAME_UP/DOT_SIZE, (SCREEN_HEIGHT-FRAME_DOWN-DOT_SIZE)/DOT_SIZE));
    }
    void draw_food ()
    {
       SDL_SetRenderDrawColor(renderer,255,69,0,255);  // orange
       SDL_Rect dot = {x,y,DOT_SIZE,DOT_SIZE};
       SDL_RenderFillRect(renderer,&dot);
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

    void draw_frame ()
    {
      SDL_Rect frame_up = {0,0,SCREEN_WIDTH,FRAME_UP};
      SDL_Rect frame_left = {0,0,FRAME_LEFT,SCREEN_HEIGHT};
      SDL_Rect frame_down = {0,SCREEN_HEIGHT-FRAME_DOWN,SCREEN_WIDTH,FRAME_DOWN};
      SDL_Rect frame_right = {SCREEN_WIDTH-FRAME_RIGHT,0,FRAME_RIGHT,SCREEN_HEIGHT};
      SDL_SetRenderDrawColor(renderer,172, 249, 255, 255);
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
    snake.up = 0;
    snake.down = 0;
    snake.right = 0;
    snake.left = 0;
}

void classic ()
{
    Game new_game;
    Snake snake;
    init_game(snake,60,50,50,50,40,50,30,50);
    snake.score = 0;
    Food food;
    food.random_generate();

    while (new_game.is_Over(snake,"classic") == false)
    {
         snake.move("classic");
         if (food.is_eaten_by(snake)==true) {
            ++snake.score;
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_frame();
         snake.draw(0,255,0);
         food.draw_food();
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
    init_game(snake,60,50,50,50,40,50,30,50);
    snake.score = 0;
    Food food;
    food.random_generate();


    while (new_game.is_Over(snake,"modern") == false)
    {
         snake.move("modern");
         if (food.is_eaten_by(snake)==true) {
            ++snake.score;
            cout << "Score: " << snake.score << endl;
            snake.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_frame();
         snake.draw(0,255,0);
         food.draw_food();
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
    init_game(snake1,60,50,50,50,40,50,30,50);
    init_game(snake2,60,100,50,100,40,100,30,100);
    snake1.score = 0;
    snake2.score = 0;
    Food food;
    food.random_generate();

    while (new_game.is_Over_svs(snake1, snake2) == false)
    {
         snake1.move("classic");
         snake2.move("classic");
         if (food.is_eaten_by(snake1)==true ) {
            ++snake1.score;
            cout << "Score1: " << snake1.score << endl;
            snake1.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         if (food.is_eaten_by(snake2)==true ) {
            ++snake2.score;
            cout << "Score2: " << snake2.score << endl;
            snake2.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_frame();
         snake1.draw(0,255,0);
         snake2.draw(115, 36, 254);
         food.draw_food();
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

int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    srand(time(0));

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
    //classic();

    //modern();

    //snake_vs_snake();

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
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

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
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



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
    void draw ()
    {
       SDL_SetRenderDrawColor(renderer,0,255,0,255);  // green
       SDL_Rect dot = {x,y,size,size};
       SDL_RenderFillRect(renderer,&dot);
    }
    void move()  // chỉ với body[0]
    {
        x+=directionX;
        y+=directionY;
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
    bool right;
    bool left;
    bool up;
    bool down;

    void draw ()
    {
        for (int i=0; i<body.size(); ++i)
        {
            body[i].draw();
        }
    }

    void move ()
    {
        for (int i=0; i<body.size(); ++i)
        {
            if (i==0) {
                body[i].x0 = body[i].x;
                body[i].y0 = body[i].y;
                body[i].move();
                //cout <<"body: " << i << ":" << body[i].x0 << " " <<  body[i].y0 << " " << body[i].x << " " <<  body[i].y << endl;
            }
            else {
               body[i].x0 = body[i].x;
               body[i].y0 = body[i].y;
               body[i].x = body[i-1].x0;
               body[i].y = body[i-1].y0;
             //  cout <<"body: " << i << ":" << body[i].x0 << " " <<  body[i].y0 << " " << body[i].x << " " <<  body[i].y << endl;
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

};

int random (int min, int max)
{
    return min+rand()%(max-min+1);
}

struct Food
{
    int x;
    int y;
    int size = DOT_SIZE;
    void random_generate ()
    {
        x = 10*(random(2,61));
        y = 10*(random(4,45));
    }
    void draw_food ()
    {
       SDL_SetRenderDrawColor(renderer,255,69,0,255);  // orange
       SDL_Rect dot = {x,y,size,size};
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
    int score;
    int high_score;

    void draw_frame ()
    {
      SDL_Rect frame_up = {0,0,SCREEN_WIDTH,40};
      SDL_Rect frame_left = {0,0,20,SCREEN_HEIGHT};
      SDL_Rect frame_down = {0,SCREEN_HEIGHT-20,SCREEN_WIDTH,20};
      SDL_Rect frame_right = {SCREEN_WIDTH-20,0,20,SCREEN_HEIGHT};
      SDL_SetRenderDrawColor(renderer,0,0,255,255);
      SDL_RenderFillRect(renderer,&frame_up);
      SDL_RenderFillRect(renderer,&frame_down);
      SDL_RenderFillRect(renderer,&frame_left);
      SDL_RenderFillRect(renderer,&frame_right);
    }

    bool is_Over (Snake &snake)
    {
       if ( (snake.body[0].inside(20,40,SCREEN_WIDTH-20,SCREEN_HEIGHT-20)==false) || (snake.eat_itself() == true) ) return true;
       // || (snake.eat_itself() == true)
       else return false;
    }
};


void init_game (Snake &snake)
{
    Dot d0(60,50);   // head
    Dot d1(50,50);
    Dot d2(40,50);
    Dot d3(30,50);
    snake.body.push_back(d0);
    snake.body.push_back(d1);
    snake.body.push_back(d2);
    snake.body.push_back(d3);
    snake.up = 0;
    snake.down = 0;
    snake.right = 0;
    snake.left = 0;
}

int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    srand(time(0));
    Game new_game;
    Snake snake;
    init_game(snake);
    new_game.score = 0;
    //snake.draw();
    Food food;
    food.random_generate();


    while (new_game.is_Over(snake) == false)
    {
         snake.move();
         if (food.is_eaten_by(snake)==true) {
            ++new_game.score;
            cout << "Score: " << new_game.score << endl;
            snake.update();  // tăng kích thước rắn
            food.random_generate();   // random thức ăn
         }
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         new_game.draw_frame();
         snake.draw();
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
         //snake.move();

    }
    cout << "Game Over";

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

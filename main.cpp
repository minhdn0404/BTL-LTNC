

#include <iostream>
#include <SDL.h>
#include <string>
#include <vector>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const string WINDOW_TITLE = "Snake game";
const int STEP = 10;

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
    int size = 20; // kích thước dot
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
       SDL_SetRenderDrawColor(renderer,0,255,0,255);
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
                 case 6: body[i].turn_right(); break;
                 case 4 : body[i].turn_left(); break;
                 case 8 : body[i].turn_up(); break;
                 case 2 : body[i].turn_down(); break;
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

};

void init_game (Snake &snake)
{
    Dot d0(40,10);   // head
    Dot d1(30,10);
    Dot d2(20,10);
    Dot d3(10,10);
    snake.body.push_back(d0);
    snake.body.push_back(d1);
    snake.body.push_back(d2);
    snake.body.push_back(d3);

}
int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    Snake snake;
    init_game(snake);
    snake.draw();

    while (snake.body[0].inside(0,0,SCREEN_WIDTH,SCREEN_HEIGHT)==true)
    {
         snake.move();
         SDL_SetRenderDrawColor(renderer,255,255,255,255);
         SDL_RenderClear(renderer);
         snake.draw();
         SDL_RenderPresent(renderer);
         SDL_Delay(30);

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

    cout << "QUITED";

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



#include "library.h"

using namespace std;

int main (int argc, char* argv[])
{
    initSDL(window, renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);

    srand(time(0));

    int check_mode = -1; // 1 - classic 2 - modern 3 - svs

    intro();

    bool program = true;
    while (program)
    {
    flag0:
        bool check_classic = false;
        bool check_modern = false;
        bool check_svs = false;
        run(check_mode, check_classic, check_modern, check_svs);

        if (check_classic == true) goto flag1;
        else if (check_modern == true) goto flag2;
        else if (check_svs == true) goto flag3;

    flag1:
            classic();
            check_mode = 1;
            goto flag4;

    flag2:
            modern();
            check_mode = 2;
            goto flag4;
    flag3:
            snake_vs_snake();
            check_mode = 3;
            goto flag4;

    flag4:
        next();

        bool play_again = false;
        bool menu = false;
        bool quit = false;
        end(check_mode, play_again, menu, quit);
        if (play_again == true)
        {
            if (check_mode == 1) goto flag1;
            else if (check_mode == 2) goto flag2;
            else if (check_mode == 3 ) goto flag3;
        }
        else if (menu == true)
        {
            goto flag0;
        }
        else if (quit == true) program = false;
    }


    //waitUntilKeyPressed();
    quitSDL(window, renderer, gFont);
    return 0;
}



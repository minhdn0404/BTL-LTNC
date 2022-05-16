
#ifndef SDL_UTIL_H
#define SDL_UTIL_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer, TTF_Font *gFont);
void waitUntilKeyPressed();

SDL_Texture* loadFromRenderedText( string textureText, SDL_Color textColor, SDL_Renderer* renderer,TTF_Font *gFont );
SDL_Texture* loadTexture( string path, SDL_Renderer* renderer);


#endif // SDL_UTIL_H

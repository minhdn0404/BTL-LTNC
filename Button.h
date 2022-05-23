
#pragma once


struct Button
{
    int x,y,w,h;
    int current_state;
    SDL_Texture* button_texture;

    Button ()
    {
        current_state = MOUSE_OUT;
        button_texture = nullptr;
    }
    ~Button()
    {
        SDL_DestroyTexture(button_texture);
        button_texture = nullptr;
    }
    void set_position (int x_, int y_, int w_, int h_)
    {
        x = x_; y = y_;
        w = w_; h = h_;
    }

    void handle_event (SDL_Event e)
    {
        // The mouse offsets
        int x_m, y_m;

        // if the mouse moved
        if (e.type == SDL_MOUSEMOTION)
        {
            x_m = e.motion.x;
            y_m = e.motion.y;

            // if mouse is over
            if (x_m > x && x_m <x+w && y_m>y && y_m<y+h)
            {
                current_state = MOUSE_OVER;
            }
            else current_state = MOUSE_OUT;
        }
        // if mouse was pressed
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x_m = e.button.x;
                y_m = e.button.y;

                if (x_m > x && x_m <x+w && y_m>y && y_m<y+h)
                {
                   current_state = MOUSE_DOWN;
                }
            }
        }
        if (e.type == SDL_MOUSEBUTTONUP)
        {
            if (e.button.button == SDL_BUTTON_LEFT)
            {
                x_m = e.button.x;
                y_m = e.button.y;
                if (x_m > x && x_m <x+w && y_m>y && y_m<y+h)
                {
                   current_state = MOUSE_UP;
                }

            }
        }
    }

    void load_image(SDL_Renderer* renderer, string path)
    {
        button_texture = loadTexture(path,renderer);
    }
    void draw(SDL_Renderer* renderer)
    {
        SDL_Rect rect = {x,y,w,h};
        SDL_RenderCopy(renderer,button_texture,nullptr,&rect);
    }
};

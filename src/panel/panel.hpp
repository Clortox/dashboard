///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// pannel definition
//
///////////////////////////////////////
// Note: 
// This class is a pure virtual class. board has an array of these, and
// each of them is a different page that will be shown. Whatever pannel is
// set as pannel_border will not check its own time elapsed, and will be
// displayed on top of all other pannels


#pragma once

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace dashboard::panel {
    class panel {
    public:
        panel() = default;
        ~panel() = default;

        virtual void draw() = 0;
    
        //in milliseconds
        size_t _time_on_screen = 0;

    protected:
        SDL_Texture* _texture;

        virtual void update() = 0;
        virtual void update_texture() = 0;
        virtual void initTexture() = 0;
    };
}

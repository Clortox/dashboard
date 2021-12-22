///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 21-12-21
// Wifi panel
//

#pragma once

#include "panel.hpp"

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <curl/curl.h>

#include <iostream>

namespace dashboard::panel {
    class wifi : public panel {
    public:
        wifi();
        ~wifi();

        static size_t curl_callback(void*, size_t, size_t, void*);

        void draw();
    private:
        void update();
        void update_texture();
        void initTexture();
    };

}

#include "../board.hpp"

///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 20-12-21
// def_overlay definition
//

#pragma once

#include "panel.hpp"

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace dashboard::panel {
    class def_overlay : public panel {
    public:
        def_overlay();
        ~def_overlay();

        void draw();


    private:
        void update();
        void update_texture();
        void initTexture();

        char date_time [100];

        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds _update_interval;
    };
}

#include "../board.hpp"

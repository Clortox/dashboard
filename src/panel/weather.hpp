///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 19-9-21
// weather panel
//

#pragma once

#include "panel.hpp"

#include <chrono>
#include "../util/rss.hpp"

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

namespace dashboard::panel {
    class weather : public panel {
    public:
        weather();
        ~weather();

        void draw();

    private:
        void update();

        rss_utils::rss _rss;
        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds  _update_interval;
    };
}

#include "../board.hpp"

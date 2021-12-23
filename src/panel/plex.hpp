///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 23-12-21
// Plex stats panel (tautilli)
//

#pragma once

#include "panel.hpp"

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <curl/curl.h>
#include "../util/rapidjson/document.h"

#include <iostream>
#include <string>
#include <chrono>

namespace dashboard::panel {
    class plex : public panel {
    public:
        plex();
        ~plex();

        static size_t curl_callback(void*, size_t, size_t, void*);

        void draw();
    private:
        void update();
        void update_texture();
        void initTexture();

        CURL* api_curl;
        std::string json_string;
        rapidjson::Document json_doc;

        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds _update_interval;
    };
}

#include "../board.hpp"

///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 14-1-22
// Home assistant panel
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
#include <string.h>
#include <chrono>
#include <vector>
#include <iomanip>

namespace dashboard::panel {
    class homeassistant : public panel {
    public:
        homeassistant();
        ~homeassistant();

        static size_t curl_callback(void*, size_t, size_t, void*);

        void draw();
    private:
        void update();
        void update_texture();
        void initTexture();

        std::time_t datestringToTm(const std::string&);

        struct home_entry {
            std::string name;
            bool home;
            std::string time;
        };

        std::vector<home_entry> home_entries;

        CURL* api_curl;
        struct curl_slist *headers;
        std::string json_string;
        rapidjson::Document json_doc;

        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds _update_interval;
    };
}

#include "../board.hpp"

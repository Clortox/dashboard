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
#include "../util/rapidjson/document.h"

#include <iostream>
#include <chrono>

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

        std::string public_ip;
        bool wan_status;
        std::string speedtest_up;
        std::string speedtest_down;
        std::string speedtest_ping;

        CURL* api_curl;
        std::string json_string;
        rapidjson::Document json_doc;

        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds _update_interval;
    };

}

#include "../board.hpp"

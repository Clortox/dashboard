///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 19-9-21
// weather panel
//

#pragma once

#include "panel.hpp"

#include <chrono>

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <curl/curl.h>
#include "../util/rapidjson/document.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

namespace dashboard::panel {
    class weather : public panel {
    public:
        weather();
        ~weather();

        static size_t curl_callback(void*, size_t, size_t, void*);

        void draw();
    private:
        void update();
        void update_texture();
        void initTexture();

        static constexpr bool TODAY = true;
        static constexpr bool TOMMOROW = false;

        std::unordered_map<std::string, 
                     std::pair<std::string, std::string>> weather_string;

        std::pair<std::string, std::string>* weather_today;
        std::pair<std::string, std::string>* weather_tommorow;
        std::pair<std::string, std::string>* weather_day_after;
        short temp_today;
        short temp_tommorow;
        short temp_day_after;

        CURL* api_curl;
        std::string json_string;
        rapidjson::Document* json_doc;

        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        std::chrono::milliseconds  _update_interval;
    };
}

#include "../board.hpp"

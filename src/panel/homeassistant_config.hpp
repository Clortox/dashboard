///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 14-1-22
// homeassistant configuration
//

#pragma once

#include "def_overlay_config.hpp"

namespace dashboard::panel {
    //This will be displayed at the top left on the status bar. Set to a blank
    //string to not show anything
    constexpr char HOMEASSISTANT_TITLE[] = "Home assistant";

    //API key, you can get this in homeassistant under your profile
    constexpr char HOMEASSISTANT_APIKEY[] = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiIxYTNhNDhhYTBlZDM0MjkyOTk5OWZhNGVjZGRjMGUwMCIsImlhdCI6MTY0MjE4NDU0MCwiZXhwIjoxOTU3NTQ0NTQwfQ.Zdy7-ZwX0HuwhmUGeLhF5XCluotsKmpDqmi5o-hQZCc";

    //This API endpoint. Sub the IP with the IP of the homeassistant server on
    //your network. Be sure to enable the RESTful API in homeassistant's
    //configuration.yaml
    constexpr char HOMEASSISTANT_URL[] = "http://192.168.1.104:8123/api/states";

    //Default time the slide is shown on screen, in ms
    //Default 15s
    constexpr size_t HOMEASSISTANT_DEFAULT_ON_SCREEN_TIME = 15000;

    //How long should we wait between updates? in ms
    //Default 15
    constexpr size_t HOMEASSISTANT_UPDATE_INTERVAL = 15000;
}

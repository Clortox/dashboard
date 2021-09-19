///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 19-9-21
// weather panel configuration
//

#pragma once

namespace dashboard::panel {
    //New York RSS feed
    static const char* WEATHER_URL_SOURCE = "http://rss.accuweather.com/rss/liveweather_rss.asp?locCode=10007";

    //Default time the slide is shown on screen, in ms
    //Default 5s
    constexpr size_t WEATHER_DEFAULT_ON_SCREEN_TIME = 5000;

    //How long should we wait between updates? in ms
    //Default 1 hour
    constexpr int    UPDATE_INTERVAL = 3600000;
}


///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 19-9-21
// weather panel configuration
//

#pragma once

#include <utility>
#include <string>
#include "def_overlay_config.hpp"

namespace dashboard::panel {
    //This will be displayed at the top left on the status bar. Set to a blank
    //string to not show anything
    constexpr char WEATHER_TITLE[] = "Weather";

    //New York RSS feed
    static const char* WEATHER_URL_SOURCE = "https://www.7timer.info/bin/civil.php?lon=41.13&lat=-81.48&unit=british&output=json&tzshift=0";

    //Default time the slide is shown on screen, in ms
    //Default 5s
    constexpr size_t WEATHER_DEFAULT_ON_SCREEN_TIME = 15000;

    //How long should we wait between updates? in ms
    //Default 1 hour
    constexpr size_t WEATHER_UPDATE_INTERVAL = 3600000;

    //Display temperatures in metric?
    //Default false
    constexpr bool WEATHER_METRIC = false;

    //The following are all of the strings that will be shown for each state of
    //weather, along with the image that will be shown
    //The struct is of form {WEATHER_STRING, WEATHER_IMAGE}
    //with weather string being the string shown on screen, and weather image
    //being the name of the preloaded image file to be shown. It is best that
    //these images be square.
    const std::pair<std::string, std::string> WEATHER_CLEAR_DAY = 
        {"Clear skies", "clearday.png"};
    const std::pair<std::string, std::string> WEATHER_CLEAR_NIGHT = 
        {"Clear skies", "clearnight.png"};

    const std::pair<std::string, std::string> WEATHER_PCLOUDY_DAY = 
        {"Slightly cloudy", "cloudyday.png"};
    const std::pair<std::string, std::string> WEATHER_PCLOUDY_NIGHT = 
        {"Slightly cloudy", "cloudynight.png"};

    const std::pair<std::string, std::string> WEATHER_MCLOUDY_DAY = 
        {"Moderately cloudy", "cloudyday.png"};
    const std::pair<std::string, std::string> WEATHER_MCLOUDY_NIGHT = 
        {"Moderately cloudy", "cloudynight.png"};

    const std::pair<std::string, std::string> WEATHER_CLOUDY_DAY = 
        {"Very cloudy", "cloudyday.png"};
    const std::pair<std::string, std::string> WEATHER_CLOUDY_NIGHT = 
        {"Very cloudy", "cloudynight.png"};

    const std::pair<std::string, std::string> WEATHER_HUMID_DAY = 
        {"Very humid", "humidday.png"};
    const std::pair<std::string, std::string> WEATHER_HUMID_NIGHT = 
        {"Very humid", "humidnight.png"};

    const std::pair<std::string, std::string> WEATHER_LRAIN_DAY = 
        {"Light rain", "lrainday.png"};
    const std::pair<std::string, std::string> WEATHER_LRAIN_NIGHT = 
        {"Light rain", "lrainnight.png"};

    const std::pair<std::string, std::string> WEATHER_RAIN_DAY = 
        {"Rain", "rainday.png"};
    const std::pair<std::string, std::string> WEATHER_RAIN_NIGHT = 
        {"Rain", "rainnight.png"};

    const std::pair<std::string, std::string> WEATHER_OSHOWER_DAY = 
        {"Overcast with showers", "shower.png"};
    const std::pair<std::string, std::string> WEATHER_OSHOWER_NIGHT = 
        {"Overcast with showers", "shower.png"};

    const std::pair<std::string, std::string> WEATHER_ISHOWER_DAY = 
        {"Moderate showers", "shower.png"};
    const std::pair<std::string, std::string> WEATHER_ISHOWER_NIGHT = 
        {"Moderate showers", "shower.png"};

    const std::pair<std::string, std::string> WEATHER_LSNOW_DAY = 
        {"Light snow", "snowday.png"};
    const std::pair<std::string, std::string> WEATHER_LSNOW_NIGHT =
        {"Light snow", "snownight.png"};

    const std::pair<std::string, std::string> WEATHER_SNOW_DAY = 
        {"Moderate snow", "snowday.png"};
    const std::pair<std::string, std::string> WEATHER_SNOW_NIGHT =
        {"Moderate snow", "snownight.png"};

    const std::pair<std::string, std::string> WEATHER_RAINSNOW_DAY =
        {"Rain and snow", "rainsnow.png"};
    const std::pair<std::string, std::string> WEATHER_RAINSNOW_NIGHT =
        {"Rain and snow", "rainsnow.png"};
}

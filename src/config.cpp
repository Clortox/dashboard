///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 9-29-21
// configuration file
//

#include "config.hpp"

// PANELS
// A list of all panels to be displayed, in order
dashboard::panel::panel* PANELS[] = {
    //new dashboard::panel::sample_panel(),
    new dashboard::panel::plex(),
    new dashboard::panel::weather(),
    new dashboard::panel::wifi(),
};
size_t PANELS_LENGTH = sizeof(PANELS)/sizeof(PANELS[0]);

// OVERLAY
// This is a special panel that is overlayed on top of all other panels
dashboard::panel::panel* OVERLAY = new dashboard::panel::def_overlay();

// IMAGE_LOCATIONS
// Locations of all static images used
const char* IMAGE_LOCATIONS[] = {
    //sample panel
    //"bmp_24.png",

    //wifi
    "wifi.png",
    "wifi_background.jpg",

    //weather
    "weather_background.png",
    "clearday.png",
    "clearnight.png",
    "cloudyday.png",
    "cloudynight.png",
    "humidday.png",
    "humidnight.png",
    "lrainday.png",
    "lrainnight.png",
    "rainday.png",
    "rainnight.png",
    "rainsnow.png",
    "shower.png",
    "snowday.png",
    "snownight.png",

    //plex
    "plex_background.jpg",
    
};
size_t IMAGE_LOCATIONS_LENGTH = sizeof(IMAGE_LOCATIONS)/sizeof(IMAGE_LOCATIONS[0]);

// FONT_LOCATIONS
// Locations of all fonts used
//struct FONT_SIZE {
//    const char* _name;
//    const size_t _size;
//};
const FONT_SIZE FONT_LOCATIONS[] = {
    { "Roboto_Mono/RobotoMono-Medium.ttf", 50 },
    { "Roboto_Mono/RobotoMono-Medium.ttf", 28 },
};
size_t FONT_LOCATIONS_LENGTH = sizeof(FONT_LOCATIONS)/sizeof(FONT_LOCATIONS[0]);

// CONST_STRINGS
// All constant strings
//struct FONT_SIZE_STRING {
//    const char* _text;
//    const FONT_SIZE _font_size;
//};
const FONT_SIZE_STRING CONST_STRINGS[] = {
    //Overlay strings
    { "Weather", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Wireless", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    //sample panel
    { "Sample Panel", { "Roboto_Mono/RobotoMono-Medium.ttf", 50} },

    //Weather strings
    { "Clear Skies", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Slightly cloudy", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Moderately cloudy", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Very cloudy", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Very humid", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Light rain", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Rain", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Overcast with showers", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Moderate showers", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Light snow", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Moderate snow", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Rain and snow", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Currently,", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Tommorow,", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },
    { "Day after,", { "Roboto_Mono/RobotoMono-Medium.ttf", 50 } },

};
size_t CONST_STRINGS_LENGTH = sizeof(CONST_STRINGS)/sizeof(CONST_STRINGS[0]);

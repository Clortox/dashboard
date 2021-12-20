///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 9-29-21
// configuration file
//

#include "config.hpp"

// PANELS
// A list of all panels to be displayed, in order
dashboard::panel::panel* PANELS[] = {
    new dashboard::panel::weather(),
};
size_t PANELS_LENGTH = sizeof(PANELS)/sizeof(PANELS[0]);

// OVERLAY
// This is a special panel that is overlayed on top of all other panels
dashboard::panel::panel* OVERLAY = new dashboard::panel::def_overlay();

// IMAGE_LOCATIONS
// Locations of all static images used
const char* IMAGE_LOCATIONS[] = {
    "bmp_24.png",
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
};
size_t FONT_LOCATIONS_LENGTH = sizeof(FONT_LOCATIONS)/sizeof(FONT_LOCATIONS[0]);

// CONST_STRINGS
// All constant strings
//struct FONT_SIZE_STRING {
//    const char* _text;
//    const FONT_SIZE _font_size;
//};
const FONT_SIZE_STRING CONST_STRINGS[] = {
    //Weather strings
    { "Today's Weather", { "Roboto_Mono/RobotoMono-Medium.ttf", 36 } },
};
size_t CONST_STRINGS_LENGTH = sizeof(CONST_STRINGS)/sizeof(CONST_STRINGS[0]);

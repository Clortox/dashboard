///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Configuration file
//

#pragma once

#include <stdint.h>
#include <SDL2/SDL_image.h>

//SCREEN_{WIDTH, HEIGHT}
// Width and height of the screen/window
constexpr int SCREEN_WIDTH  = 1920;
constexpr int SCREEN_HEIGHT = 1080;

// MAX_FRAMERATE
// The aprox framerate at runtime
constexpr int MAX_FRAMERATE = 60;

// WINDOW_TITLE
// Title of the default window. Does not matter if not using X11
constexpr char WINDOW_TITLE[] = "Dashboard";

// BOARD_{RED, GREEN, BLUE}
// Default color used
constexpr uint8_t BOARD_RED   = 0xFF;
constexpr uint8_t BOARD_GREEN = 0xFF;
constexpr uint8_t BOARD_BLUE  = 0xFF;

// SDL_FLAGS
// SDL init() flags
constexpr uint32_t SDL_FLAGS = 0
    | SDL_INIT_VIDEO
    //| SDL_INIT_AUDIO
    //| SDL_INIT_TIMER
    //| SDL_INIT_EVENTS
    ;

// SDL_WINDOW_FLAGS
// SDL init window flags
constexpr uint32_t SDL_WINDOW_FLAGS = 0
    //| SDL_WINDOW_FULLSCREEN
    | SDL_WINDOW_FULLSCREEN_DESKTOP

    | SDL_WINDOW_SHOWN
    // | SDL_WINDOW_HIDDEN

    | SDL_WINDOW_OPENGL
    //| SDL_WINDOW_VULKAN
    //| SDL_WINDOW_METAL

    | SDL_WINDOW_BORDERLESS
    //| SDL_WINDOW_RESIZEABLE
    //| SDL_WINDOW_MINIMIZED
    //| SDL_WINDOW_MAXIMIZED
    ;

// SDL_SHOW_CURSOR
// Toggle if the cursor is shown on screen
constexpr int SDL_SHOW_CURSOR = 0
    | SDL_ENABLE
    // | SDL_DISABLE
    ;

// IMG_FLAGS
// Enable support for different image formats
constexpr int IMG_FLAGS = 0
    | IMG_INIT_JPG
    | IMG_INIT_PNG
    //| IMG_INIT_TIF
    ;

// PANELS
// A list of all panels to be displayed, in order
// Be sure to include the panel definition below, then add it to the system
#include "panel/panel.hpp"
#include "panel/weather.hpp"
static dashboard::panel::panel* PANELS[] = {
    new dashboard::panel::weather(),

};

// DATA_DIR
// Where all resources will be
// Keep this as DATA to use the install dir set in the makefile
constexpr char DATA_DIR[]  = DATA_ ;

// IMG_DIR
// Where all images are
// Keep this a DATA_IMG_ to use the DATA_IMG dir defined in the makefile
constexpr char DATA_IMG[] = DATA_IMG_;

// IMAGE_LOCATIONS
// Locations of all static images used
static const char* IMAGE_LOCATIONS[] = {
    "bmp_24.png",
    //Weather
    "sunny.png",
    "rainy.png",
};

// FONT_DIR
// Where all fonts are kept
// Keep this as FONT_DIR_ to use the DATA_FONT dir defined in the makefile
constexpr char DATA_FONT[] = DATA_FONT_;

// FONT_LOCATIONS
// Locations of all fonts used
struct FONT_SIZE {
    const char* _name;
    const size_t _size;
};
static const FONT_SIZE FONT_LOCATIONS[] = {
    { "Roboto_Mono/RobotoMono-Medium.ttf", 24 },
};

// CONST_STRINGS
// All constant strings
struct FONT_SIZE_STRING {
    const char* _text;
    const FONT_SIZE _font_size;
};
static const FONT_SIZE_STRING CONST_STRINGS[] = {
    //Weather strings
    { "Today's Weather", { "Roboto_Mono/RobotoMono-Medium.ttf", 24 } },
    { "Sunny"          , { "Roboto_Mono/RobotoMono-Medium.ttf", 24 } },
    { "Rainy"          , { "Roboto_Mono/RobotoMono-Medium.ttf", 24 } },
};

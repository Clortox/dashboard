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
constexpr int SCREEN_HEIHGT = 1080;

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

// DATA_DIR
// Where all resources will be
// Keep this as DATA to use the install dir set in the makefile
constexpr char DATA_DIR[]  = DATA;

// IMAGE_DIR
// Where all images will be
// Keep this as DATA_IMG to use img dir set in the makefile
constexpr char IMAGE_DIR[] = DATA_IMG;

// FONT_DIR
// Where all fonts will be
// Keep this as DATA_FONT to use font dir set in the makefile
constexpr char FONT_DIR[]  = DATA_FONT;

// IMAGE_LOCATIONS
// Locations of all static images used
static const char* IMAGE_LOCATIONS[] = {

};

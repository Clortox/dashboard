///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 20-12-21
// def overlay configuration
//

#pragma once

#include <SDL.h>

namespace dashboard::panel {
    //How long should we wait between updates? in ms
    //Default 1s
    constexpr size_t DEF_OVERLAY_UPDATE_INTERVAL = 1000;

    //Height of the bar on the top and bottom, in pixels
    constexpr size_t DEF_OVERLAY_BAR_HEIGHT = 60;

    //Text color
    constexpr uint8_t DEF_OVERLAY_TEXT_RED    = 0xCC;
    constexpr uint8_t DEF_OVERLAY_TEXT_GREEN  = 0xCC;
    constexpr uint8_t DEF_OVERLAY_TEXT_BLUE   = 0xCC;
    constexpr uint8_t DEF_OVERLAY_TEXT_ALPHA  = 0xFF;

    constexpr uint8_t DEF_OVERLAY_BAR_RED     = 0xD3;
    constexpr uint8_t DEF_OVERLAY_BAR_GREEN   = 0xD3;
    constexpr uint8_t DEF_OVERLAY_BAR_BLUE    = 0xD3;
    constexpr uint8_t DEF_OVERLAY_BAR_ALPHA   = 0x7F;

    //for more on how this string format works, refer to
    //https://en.cppreference.com/w/cpp/chrono/c/strftime
    constexpr char DEF_OVERLAY_DATE_STRING[]  = "%a %R %p";
}

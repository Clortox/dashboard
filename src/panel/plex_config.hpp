///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 23-22-21
// Plex panel configuration
//

#pragma once

#include "def_overlay_config.hpp"

namespace dashboard::panel {
    //This will be displayed at the top left on the status bar. Set to a blank
    //string to not show anything
    constexpr char PLEX_TITLE[] = "Plex";

    //Tautili endpoint 
    constexpr char PLEX_URL_SOURCE[] = "http://192.168.1.104:8181/api/v2?apikey=64af06e0497342f7a5862462ddbbd309&cmd=get_history&length=5";

    //How many characters of a show title should we show?
    constexpr size_t PLEX_MAX_STRING_LENGTH = 35;

    //Default time the slid is shown on screen, in ms
    //Default 15s
    constexpr size_t PLEX_DEFAULT_ON_SCREEN_TIME = 15000;

    //How long should we wait between updates? in ms
    //Default 15
    constexpr size_t PLEX_UPDATE_INTERVAL = 15000;

    //Color for background bounding boxes
    constexpr uint8_t PLEX_BGBOX_RED     = 0x66;
    constexpr uint8_t PLEX_BGBOX_GREEN   = 0x66;
    constexpr uint8_t PLEX_BGBOX_BLUE    = 0x66;
    constexpr uint8_t PLEX_BGBOX_ALPHA   = 0x99;

}

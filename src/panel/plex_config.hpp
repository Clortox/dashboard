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
    constexpr char PLEX_URL_SOURCE_HISTORY[] = "http://192.168.1.104:8181/api/v2?apikey=62917cc0bea04aa69cbb85141e312e84&cmd=get_history&length=5";
    constexpr char PLEX_URL_SOURCE_NAME[] = "http://192.168.1.104:8181/api/v2?apikey=62917cc0bea04aa69cbb85141e312e84&cmd=get_server_friendly_name";
    constexpr char PLEX_URL_SOURCE_TOP_USERS[] = "http://192.168.1.104:8181/api/v2?apikey=62917cc0bea04aa69cbb85141e312e84&cmd=get_plays_by_top_10_users";

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

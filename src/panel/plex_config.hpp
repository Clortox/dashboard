///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 23-22-21
// Plex panel configuration
//

#pragma once

namespace dashboard::panel {
    //This will be displayed at the top left on the status bar. Set to a blank
    //string to not show anything
    constexpr char PLEX_TITLE[] = "Plex";

    //Tautili endpoint 
    constexpr char PLEX_URL_SOURCE[] = "http://192.168.1.104:8181/api/v2?apikey=<KEY_GOES_HERE>&cmd=";

    //Default time the slid is shown on screen, in ms
    //Default 15s
    constexpr size_t PLEX_DEFAULT_ON_SCREEN_TIME = 15000;

    //How long should we wait between updates? in ms
    //Default 30s
    constexpr size_t PLEX_UPDATE_INTERVAL = 30000;

}

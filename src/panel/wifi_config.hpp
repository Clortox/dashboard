///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 21-12-21
// def overlay configuration
//

#pragma once

namespace dashboard::panel {
    //Default time the slid is shown on the screen, in ms
    //Default 15s
    constexpr size_t WIFI_DEFAULT_TIME_ON_SCREEN = 15000;

    //How long should we wait between updates? in ms
    //Due to the nature of this panel, this value is ignored
    constexpr size_t WIFI_UPDATE_INTERVAL = 60000;

    //All of the following options WILL be shown on the screen.
    //If this is a privacy concernt for you, set the value to be blank
    //(ie "") and it will not be shown
    constexpr char WIFI_NETWORK_NAME[] = "MyNetwork";
    constexpr char WIFI_NETWORK_PASS[] = "MyPassword";
}

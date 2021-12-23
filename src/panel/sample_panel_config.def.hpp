///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 22-12-21
// sample panel configuration
//

#pragma once

namespace dashboard::panel {
    //This will be displayed at the top left of the status bar. Set to a blank
    //string to not shown anything
    constexpr char SAMPLE_PANEL_TITLE[] = "Sample Panel";

    //Default time the slide is shown on the screen, in ms
    //Default 15s
    constexpr size_t SAMPLE_PANEL_TIME_ON_SCREEN = 15000;

    //How long should we wait between updates? in ms
    //NOTE this is a minimum and update() is not guarenteed to run at this
    //exact interval
    //Default 30s
    constexpr size_t SAMPLE_PANEL_UPDATE_INTERVAL = 30000;

    //Should we double the value each time as well?
    constexpr bool SAMPLE_PANEL_DOUBLE_VAL = true;

}

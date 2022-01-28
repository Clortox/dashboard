///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 21-12-21
// def overlay configuration
//

#pragma once

namespace dashboard::panel {
    //This will be displayed at the top left on the status bar. Set to a blank
    //string to not show anything
    constexpr char WIFI_TITLE[] = "Wireless";

    //Default time the slid is shown on the screen, in ms
    //Default 15s
    constexpr size_t WIFI_DEFAULT_TIME_ON_SCREEN = 15000;

    //How long should we wait between updates? in ms
    //Default 60s
    constexpr size_t WIFI_UPDATE_INTERVAL = 60000;

    //Set to true to have your public IP be shown on the wifi page
    //Will make a curl request to WIFI_PUBLIC_IP_URL when the frame
    //is rendered. This this is a privacy concern disable this option
    constexpr bool WIFI_SHOW_PUBLIC_IP = true;
    constexpr char WIFI_PUBLIC_IP_URL[] = "http://ipinfo.io/ip";

    //All of the following options WILL be shown on the screen.
    //If this is a privacy concernt for you, set the value to be blank
    //(ie "") and it will not be shown
    constexpr char WIFI_NETWORK_NAME[] = "MyNetwork";
    constexpr char WIFI_NETWORK_PASS[] = "MyPassword";

    // Set to true if you have home assistant on your network
    // If you do, you can do alot more with it, including showing number of
    // clients, network speed, etc
    constexpr bool WIFI_HOMEASSISTANT = true;

    //API and URL for home assistant. If using the home assistant panel, this
    //should be the same
    constexpr char WIFI_HOMEASSISTANT_APIKEY[] = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiIxYTNhNDhhYTBlZDM0MjkyOTk5OWZhNGVjZGRjMGUwMCIsImlhdCI6MTY0MjE4NDU0MCwiZXhwIjoxOTU3NTQ0NTQwfQ.Zdy7-ZwX0HuwhmUGeLhF5XCluotsKmpDqmi5o-hQZCc";
    constexpr char WIFI_HOMEASSISTANT_URL[] = "http://192.168.1.104:8123/api/states";

    //Name of the external IP sensor entry in home assistant
    //If your router integrates with it, then you should be able to find it in
    //integrations
    constexpr char WIFI_HOMEASSISTANT_ROUTER_EXTERNAL_IP[] = "sensor.r7000p_gateway_external_ip";

    //Name of WAN status sensor in home assistant
    constexpr char WIFI_HOMEASSISTANT_ROUTER_WAN_STATUS[] = "sensor.r7000p_gateway_wan_status";

    //Enable this if you have the ookla speedtest module on home assistant
    constexpr bool WIFI_HOMEASSISTANT_SPEEDTEST = true;

    //Name of the entity id string used by the speedtest upload sensor in home
    //assistant
    constexpr char WIFI_HOMEASSISTANT_SPEEDTEST_UP[] = "sensor.speedtest_upload";

    //Name of the entity id string used by the speedtest download sensor in home
    //assistant
    constexpr char WIFI_HOMEASSISTANT_SPEEDTEST_DOWN[] = "sensor.speedtest_download";

    //Name of the entity id string used by the speedtest ping sensor in home
    //assistant
    constexpr char WIFI_HOMEASSISTANT_SPEEDTEST_PING[] = "sensor.speedtest_ping";
}

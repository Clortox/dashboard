///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 21-12-21
// wifi panel
//

#include "wifi.hpp"
#include "wifi_config.hpp"

#include "def_overlay_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

wifi::wifi(){
    std::cerr << "WIFI CONSTRUCTOR\n";
    _texture = nullptr;
    _time_on_screen = WIFI_DEFAULT_TIME_ON_SCREEN;
    _update_interval = std::chrono::milliseconds{WIFI_UPDATE_INTERVAL};
    _title = WIFI_TITLE;

    api_curl = curl_easy_init();
    curl_easy_setopt(api_curl, CURLOPT_WRITEFUNCTION,
            dashboard::panel::wifi::curl_callback);
    curl_easy_setopt(api_curl, CURLOPT_WRITEDATA, &json_string);
}

wifi::~wifi(){
    std::cerr << "WIFI DECONSTRUCTOR\n";
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
    if(api_curl != nullptr)
        curl_easy_cleanup(api_curl);
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void wifi::draw(){
    //create the texture if this is the first time running draw
    if(_texture == nullptr){
        initTexture();
        update();
        update_texture();
    }

    //check if its time to update
    if((std::chrono::high_resolution_clock::now() - _last_update)
            > _update_interval){
        update();

        update_texture();
    }

    SDL_RenderCopy(board::getRenderer(), _texture, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Update the information of wifi
// This DOES NOT update the display
void wifi::update() {
    std::cerr << "WIFI::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    if(WIFI_HOMEASSISTANT){
        curl_easy_setopt(api_curl, CURLOPT_URL,
                WIFI_HOMEASSISTANT_URL);
        std::string api_string = "Authorization: Bearer ";
        api_string += WIFI_HOMEASSISTANT_APIKEY;

        struct curl_slist *headers;
        headers = NULL;
        headers = curl_slist_append(headers, api_string.c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(api_curl, CURLOPT_HTTPHEADER, headers);

        api_string.clear();

        //perform request
        curl_easy_perform(api_curl);

        //parse the result
        json_doc.Parse(json_string.c_str());

        //update internal state
        for(rapidjson::SizeType i = 0; i < json_doc.Size(); i++){
            //if router external IP
            if(strcmp(json_doc[i]["entity_id"].GetString(),
                        WIFI_HOMEASSISTANT_ROUTER_EXTERNAL_IP) == 0
                    && WIFI_SHOW_PUBLIC_IP){
                //store it in public IP field
                public_ip = "WAN IP: ";
                public_ip += json_doc[i]["state"].GetString();
            } //if router WAN status
            else if (strcmp(json_doc[i]["entity_id"].GetString(),
                        WIFI_HOMEASSISTANT_ROUTER_WAN_STATUS) == 0){
                if(strcmp(json_doc[i]["state"].GetString(), "Connected") == 0)
                    wan_status = true;
                else
                    wan_status = false;
            } //speedtest up
            else if (strcmp(json_doc[i]["entity_id"].GetString(),
                        WIFI_HOMEASSISTANT_SPEEDTEST_UP) == 0
                    && WIFI_HOMEASSISTANT_SPEEDTEST){
                speedtest_up = "Upload: ";
                speedtest_up += json_doc[i]["state"].GetString();
                speedtest_up += "Mbps";
            } //speedtest down
            else if (strcmp(json_doc[i]["entity_id"].GetString(),
                        WIFI_HOMEASSISTANT_SPEEDTEST_DOWN) == 0
                    && WIFI_HOMEASSISTANT_SPEEDTEST){
                speedtest_down = "Download: ";
                speedtest_down += json_doc[i]["state"].GetString();
                speedtest_down += "Mbps";
            } //speedtest ping
            else if (strcmp(json_doc[i]["entity_id"].GetString(),
                        WIFI_HOMEASSISTANT_SPEEDTEST_PING) == 0
                    && WIFI_HOMEASSISTANT_SPEEDTEST){
                speedtest_ping = "Ping: ";
                speedtest_ping += json_doc[i]["state"].GetString();
                speedtest_ping += "ms";
            }
        }
    }

    //if not using home assistant, grab it via normal method
    if(WIFI_SHOW_PUBLIC_IP){
        //get the string from normal url
        if(!WIFI_HOMEASSISTANT){
            public_ip = "WAN IP: ";
            curl_easy_setopt(api_curl, CURLOPT_URL,
                    WIFI_PUBLIC_IP_URL);

            //peform request
            curl_easy_perform(api_curl);

            //parse the result
            public_ip += json_string;
        }
        //the public IP should have already been grabbed above
    }
}

///////////////////////////////////////
// Update the texture that is being
// displayed. Due to the nature of what
// is being displayed, this will only
// ever be called once, as the wifi
// info doesnt change
void wifi::update_texture(){
    std::cerr << "WIFI::UPDATE_TEXTURE\n";
    SDL_Rect tgt;

    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());

    //place background image
    SDL_RenderCopy(board::getRenderer(),
            board::getImage("wifi_background.jpg"), NULL, NULL);

    //Note about the strings here
    //Because this is only called once, it makes more sense to not have
    //any of these as static strings and instead generate them dynamically once,
    //copy it into this _texture, and then delete it once it goes out of the
    //LRU. This way, we dont waste memory holding a string we only copy once
    SDL_RenderCopy(board::getRenderer(),
            board::getString("Wireless",
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    //show the QRCode
    tgt.x = -25;
    tgt.y = DEF_OVERLAY_BAR_HEIGHT;
    tgt.w = (SCREEN_WIDTH / 2) ;
    tgt.h = tgt.w;
    SDL_RenderCopy(board::getRenderer(),
            board::getImage("wifi.png"), NULL, &tgt);

    std::string network_string("Network: ");
    network_string += WIFI_NETWORK_NAME;

    //show info about the network
    tgt.x = (SCREEN_WIDTH / 2) + 25;
    tgt.y = 60;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            network_string.c_str(), &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(network_string.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    tgt.y += tgt.h + 25;
    std::string password_string("Password: ");
    password_string += WIFI_NETWORK_PASS;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            password_string.c_str(), &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(password_string.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    tgt.y += tgt.h + 25;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            wan_status ? "Internet is up" : "Internet is down; reconnecting", &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(wan_status ? "Internet is up" : "Internet is down; reconnecting",
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    //Get public ip address and display it
    if(WIFI_SHOW_PUBLIC_IP){
        tgt.y += tgt.h + 25;
        TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
                public_ip.c_str(), &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(public_ip.c_str(),
                    { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    }

    //TODO display speedtest info
    tgt.y += tgt.h + 25;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            speedtest_ping.c_str(), &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(speedtest_ping.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    tgt.y += tgt.h + 25;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            speedtest_up.c_str(), &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(speedtest_up.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    tgt.y += tgt.h + 25;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            speedtest_down.c_str(), &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(speedtest_down.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);



    SDL_SetRenderTarget(board::getRenderer(), NULL);
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
void wifi::initTexture(){
    std::cerr << "WIFI INIT TEXTURE\n";
    if(_texture == nullptr){
        _texture = SDL_CreateTexture(board::getRenderer(),
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    }
}

///////////////////////////////////////
// Curl callback function
size_t dashboard::panel::wifi::curl_callback(void* contents, size_t size,
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

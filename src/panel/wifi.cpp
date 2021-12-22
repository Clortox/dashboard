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
    _title = WIFI_TITLE;
}

wifi::~wifi(){
    std::cerr << "WIFI DECONSTRUCTOR\n";
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
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

    SDL_RenderCopy(board::getRenderer(), _texture, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Update the information of wifi
// This DOES NOT update the display
void wifi::update() {
    //this shows static info on the wifi network, which does need to change 
    //therefore, no update
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

    //Get public ip address and display it
    if(WIFI_SHOW_PUBLIC_IP){
        std::string public_ip = "WAN IP: ";
        CURL* curl;
        curl = curl_easy_init();

        if(curl){
            CURLcode res;
            curl_easy_setopt(curl, CURLOPT_URL, WIFI_PUBLIC_IP_URL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 
                    dashboard::panel::wifi::curl_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &public_ip);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        } else {
            public_ip += "Unkown";
        }

        tgt.y += tgt.h + 25;
        TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
                public_ip.c_str(), &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(public_ip.c_str(),
                    { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    }


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

size_t dashboard::panel::wifi::curl_callback(void* contents, size_t size, 
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

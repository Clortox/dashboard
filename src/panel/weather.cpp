///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 19-9-21
// weather panel
//

#include "weather.hpp"
#include "weather_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

weather::weather(){
    std::cerr << "WEATHER CONSTRUCTOR\n";
    _time_on_screen = WEATHER_DEFAULT_ON_SCREEN_TIME;
    _update_interval = std::chrono::milliseconds{UPDATE_INTERVAL};
    //let set to default, will make it so it updates the texture ASAP
    //_last_update;
    _rss = rss_utils::rss(WEATHER_URL_SOURCE);
}

weather::~weather(){
    std::cerr << "WEATHER DECONSTRUCTOR\n";
    if(_texture != nullptr){
        SDL_DestroyTexture(_texture);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void weather::draw(){
    //create the texture if this is the first time running draw
    if(_texture == nullptr){
        initTexture();
        update();
        update_texture();
    }

    //check if its time to update
    if((std::chrono::high_resolution_clock::now() - _last_update) 
            > _update_interval){
        //TODO multithread this
        update();

        update_texture();
    }

    SDL_RenderCopy(board::getRenderer(), _texture, NULL, NULL);


    //TODO add this all to one texture

}

///////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Update the information of the obj
// This DOES NOT update the display
void weather::update() {
    std::cerr << "WEATHER::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //fetch updates
    _rss.update();

    //update internal state

    current_desc = _rss.getItem(0).getDescription();
    current_desc = current_desc.substr(0,current_desc.find('<'));
    std::cerr << "Current Description : (\" " << current_desc << "\")\n";

    tommorow_desc = _rss.getItem(1).getDescription();
    tommorow_desc = tommorow_desc.substr(0,tommorow_desc.find('<'));
    std::cerr << "Tommorow Description : (\" " << tommorow_desc << "\")\n";
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in 
// _rss
void weather::update_texture(){
    std::cerr << "WEATHER::UPDATE_TEXTURE\n";
    SDL_Rect tgt;

    SDL_SetRenderTarget(board::getRenderer(), _texture);

    //title
    tgt.x = 50;
    tgt.y = 50;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 36 }),
            "Today's Weather",
            &tgt.w, &tgt.h);
    SDL_RenderCopy(board::getRenderer(), 
            board::getString("Today's Weather", 
                { "Roboto_Mono/RobotoMono-Medium.ttf", 36 }), NULL, &tgt);

    //current weather
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 36 }),
            current_desc.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = SCREEN_WIDTH / 2 - (tgt.w / 2);
    tgt.y = SCREEN_HEIGHT / 2 - (tgt.h / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(current_desc.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 36 }), NULL, &tgt);

    //tommorow's weather
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 36 }),
            tommorow_desc.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = SCREEN_WIDTH / 2 - (tgt.w / 2);
    tgt.y = SCREEN_HEIGHT / 2 - (tgt.h / 2) + 30;
    SDL_RenderCopy(board::getRenderer(),
            board::getString(tommorow_desc.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 36 }), NULL, &tgt);

    SDL_SetRenderTarget(board::getRenderer(), NULL);
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
void weather::initTexture(){
    if(_texture == nullptr){
        _texture = SDL_CreateTexture(board::getRenderer(),
                SDL_PIXELFORMAT_RGBA8888, 
                SDL_TEXTUREACCESS_TARGET, 
                SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_NONE);
    }
}

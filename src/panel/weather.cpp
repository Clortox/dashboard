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
    _last_update = std::chrono::high_resolution_clock::now();
    _rss = rss_utils::rss(WEATHER_URL_SOURCE);

    _texture = SDL_CreateTexture(board::getRenderer(),
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            SCREEN_WIDTH, SCREEN_HEIGHT);

    update();
}

weather::~weather(){
    std::cerr << "WEATHER DECONSTRUCTOR\n";
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void weather::draw(){
    //check if its time to update
    if((std::chrono::high_resolution_clock::now() - _last_update) 
            > _update_interval){
        //TODO multithread this
        update();

        update_texture();
    }

    static SDL_Rect tgt = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    //SDL_RenderCopy(board::getRenderer(), _texture, NULL, 
            //&tgt);

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
    
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in 
// _rss
void weather::update_texture(){
    //SDL_SetRenderTarget(board::getRenderer(), _texture);

    SDL_Rect tgt;
    tgt.x = 50;
    tgt.y = 50;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 24 }),
            _rss.getTitle().c_str(),
            &tgt.w, &tgt.h);
    
    SDL_RenderCopy(board::getRenderer(), 
            board::getString(_rss.getTitle(), 
                { "Roboto_Mono/RobotoMono-Medium.ttf", 24 }), NULL, &tgt );

    //SDL_SetRenderTarget(board::getRenderer(), NULL);
}

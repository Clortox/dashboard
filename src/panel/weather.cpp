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
    _time_on_screen = WEATHER_DEFAULT_ON_SCREEN_TIME;
    _update_interval = std::chrono::milliseconds{UPDATE_INTERVAL};
    _last_update = std::chrono::high_resolution_clock::now();
    _rss = rss_utils::rss(WEATHER_URL_SOURCE);

    update();
}

weather::~weather(){

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
    }

    //TODO add this all to one canvas thing?
    //BEGIN GRAPHICS
    SDL_Rect tgt;
    tgt.x = 50;
    tgt.y = 50;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 24 }),
            _rss.getTitle().c_str(),
            &tgt.w, &tgt.h);
    
    SDL_RenderCopy(board::getRenderer(), 
            board::getString(_rss.getTitle(), 
                { "Roboto_Mono/RobotoMono-Medium.ttf", 24 }), NULL, &tgt );



    //END GRAPHICS

    SDL_RenderPresent(board::getRenderer());

}

///////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void weather::update() {
    std::cerr << "WEATHER::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //fetch updates
    _rss.update();
    
}

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
    //std::cerr << "Current Renderer : " << board::getRenderer() << "\n";
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
    std::cerr << "WEATHER::DRAW\n";

    //create the texture if this is the first time running draw
    if(_texture == nullptr)
        initTexture();

    //check if its time to update
    if((std::chrono::high_resolution_clock::now() - _last_update) 
            > _update_interval){
        std::cerr << "UPDATING WEATHER IF STATEMENT\n";
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
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in 
// _rss
void weather::update_texture(){
    std::cerr << "WEATHER::UPDATE_TEXTURE\n";

    int ret = SDL_SetRenderTarget(board::getRenderer(), _texture);
    std::cerr << "ret : " << ret << "\n";
    std::cerr << "Renderer : " << board::getRenderer() << "\n";
    if(ret != 0)
        SDL_Log("ERROR : %s\n", SDL_GetError());


    SDL_Rect tgt;

    //title
    tgt.x = 50;
    tgt.y = 50;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 24 }),
            _rss.getTitle().c_str(),
            &tgt.w, &tgt.h);

    std::cerr << "tgt.w : " << tgt.w << "\n";
    std::cerr << "tgt.h : " << tgt.h << "\n";
    std::cerr << "board::getString : " << board::getString(_rss.getTitle(),{ "Roboto_Mono/RobotoMono-Medium.ttf", 24 }) << "\n";

    ret = SDL_RenderCopy(board::getRenderer(), 
            //board::getString(_rss.getTitle(), 
            board::getString(_rss.getTitle(), 
                { "Roboto_Mono/RobotoMono-Medium.ttf", 24 }), NULL, &tgt);

    std::cerr << "ret : " << ret << "\n";

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

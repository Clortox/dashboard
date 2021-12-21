///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 21-12-21
// wifi panel
//

#include "wifi.hpp"
#include "wifi_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

wifi::wifi(){
    std::cerr << "WIFI CONSTRUCTOR\n";
    _texture = nullptr;
    _time_on_screen = WIFI_DEFAULT_TIME_ON_SCREEN;
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

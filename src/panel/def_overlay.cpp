///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 20-12-21
// def_overlay implementation
//

#include "def_overlay.hpp"
#include "def_overlay_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

def_overlay::def_overlay(){
    std::cerr << "DEF_OVERLAY CONSTRUCTOR\n";
    //dont init time on screen, its always on screen
    _time_on_screen = 0;
    _update_interval = std::chrono::milliseconds{DEF_OVERLAY_UPDATE_INTERVAL};
    _texture = nullptr;
    //let set to default, will make it so it updates the texture ASAP
    //_last_update;
}

def_overlay::~def_overlay(){
    std::cerr << "DEF_OVERLAY DECONSTRUCTOR\n";
    if(_texture != nullptr){
        SDL_DestroyTexture(_texture);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void def_overlay::draw(){
    //create the texture if this is the first time running draw
    if(_texture == nullptr){
        std::cerr << "IN FIRST SETUP\n";
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
// Update the information of the obj
// This DOES NOT update the display
void def_overlay::update() {
    std::cerr << "DEF_OVERLAY::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //get current date and time
    std::time_t curr_time = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
    date_time = std::ctime(&curr_time);
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on system data
void def_overlay::update_texture() {
    std::cerr << "DEF_OVERLAY::UPDATE_TEXTURE\n";
    uint8_t o_red, o_green, o_blue, o_alpha;
    SDL_Rect tgt;

    //save the old colors
    SDL_GetRenderDrawColor(board::getRenderer(), &o_red,
            &o_green, &o_blue, &o_alpha);

    SDL_SetRenderTarget(board::getRenderer(), _texture);

    //clear the texture as pure alpha
    SDL_SetRenderDrawColor(board::getRenderer(),
            0x00, 0x00, 0x00, 0x00);

    //set the new color
    SDL_SetRenderDrawColor(board::getRenderer(),
            DEF_OVERLAY_BAR_RED, DEF_OVERLAY_BAR_GREEN, 
            DEF_OVERLAY_BAR_BLUE, DEF_OVERLAY_BAR_ALPHA);

    //draw the top rectangle
    tgt.x = 0; tgt.y = 0;
    tgt.w = SCREEN_WIDTH; 
    tgt.h = DEF_OVERLAY_BAR_HEIGHT;
    SDL_RenderFillRect(board::getRenderer(), &tgt);

    //draw the bottom rectangle
    tgt.x = 0; tgt.y = SCREEN_HEIGHT - DEF_OVERLAY_BAR_HEIGHT;
    tgt.w = SCREEN_WIDTH;
    tgt.h = DEF_OVERLAY_BAR_HEIGHT;
    SDL_RenderFillRect(board::getRenderer(), &tgt);







    SDL_SetRenderTarget(board::getRenderer(), NULL);

    //reset back to the old render color
    SDL_SetRenderDrawColor(board::getRenderer(),
            o_red, o_green, o_blue, o_alpha);

    std::cerr << "Leaving overlay texture\n";
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
void def_overlay::initTexture(){
    if(_texture == nullptr){
        _texture = SDL_CreateTexture(board::getRenderer(),
                SDL_PIXELFORMAT_RGBA8888, 
                SDL_TEXTUREACCESS_TARGET, 
                SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    }
}

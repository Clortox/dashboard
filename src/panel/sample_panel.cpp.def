///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 22-12-21
// Sample panel cpp
//

#include "sample_panel.hpp"
#include "sample_panel_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

sample_panel::sample_panel(){
    std::cerr << "SAMPLE_PANEL CONSTRUCTOR\n";
    _texture = nullptr;
    _time_on_screen = SAMPLE_PANEL_TIME_ON_SCREEN;
    _update_interval = std::chrono::milliseconds{SAMPLE_PANEL_UPDATE_INTERVAL};
    _title = SAMPLE_PANEL_TITLE;
}

sample_panel::~sample_panel(){
    std::cerr << "SAMPLE_PANEL DECONSTRUCTOR\n";
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void sample_panel::draw(){
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

    //copy the local _texture to the rendering canvas
    SDL_RenderCopy(board::getRenderer(), _texture, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Update the information of the obj
// This DOES NOT update the display
void sample_panel::update(){
    std::cerr << "SAMPLE_PANEL::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //TODO Add your code to update the state of this panel here

    //Sample code
    i++;

    if(SAMPLE_PANEL_DOUBLE_VAL)
        i = i * 2;
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data that was
// grabed by update()
void sample_panel::update_texture(){
    std::cerr << "SAMPLE_PANEL::UPDATE_TEXTURE\n";

    //set the render target to our local _texture
    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());

    //TODO Add your code to update the texture here
    
    // Begin Sample code //////////////
    SDL_Rect tgt;
    std::string i_str = std::to_string(i);
    
    //Get the size of the string to be shown, and store it in tgt
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            i_str.c_str(), 
            &tgt.w, &tgt.h);
    //Set the position of the text by changing tgt.x and tgt.y
    //NOTE 0,0 is the top left corner of both textures and the screen
    tgt.x = (SCREEN_WIDTH / 2) - (tgt.w / 2); //center the text on the x axis
    tgt.y = (SCREEN_HEIGHT / 2) - (tgt.h / 2);

    //Get a texture of the string and place it on the screen
    SDL_RenderCopy(board::getRenderer(),
            board::getString(i_str.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            NULL, &tgt);
    
    //NOTE board::getString will generate a texture of that string and place it
    //in a cache. After so many textures have been generated, the texture will
    //be deleted (LRU cache). If you have a static string that you wish to add, 
    //be sure to add it to config.cpp as a static string and it will be generated 
    //and cached for the lifetime of the program

    //copy a texture at the right hand size of the screen
    tgt.x = (3*SCREEN_WIDTH / 4);
    tgt.x = (SCREEN_HEIGHT / 2) - 100;
    tgt.h = 100;
    tgt.w = 100;
    SDL_RenderCopy(board::getRenderer(),
            board::getImage("bmp_24.png"),
            NULL, &tgt);
    
    //NOTE board::getImage can only get images that were declared at runtime as
    //of this writing. To add these images place them in the img folder and add
    //the name to config.cpp


    // End Sample Code ////////////////

    //reset the render texture
    SDL_SetRenderTarget(board::getRenderer(), NULL);
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
// Most of this is fine as defaults and you probably dont want to change any of
// this
void sample_panel::initTexture(){
    std::cerr << "SAMPLE PANEL INIT TEXTURE\n";
    if(_texture == nullptr){
        _texture = SDL_CreateTexture(board::getRenderer(),
                SDL_PIXELFORMAT_RGBA8888, 
                SDL_TEXTUREACCESS_TARGET, 
                SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    }
}

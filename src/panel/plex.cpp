///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 23-12-21
// Plex stats panel (tautilli)
//

#include "plex.hpp"
#include "plex_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

plex::plex(){
    std::cerr << "PLEX CONSTRUCTOR\n";
    _time_on_screen = PLEX_DEFAULT_ON_SCREEN_TIME;
    _update_interval = std::chrono::milliseconds{PLEX_UPDATE_INTERVAL};
    _texture = nullptr;
    _title = PLEX_TITLE;

    api_curl = curl_easy_init();
    curl_easy_setopt(api_curl, CURLOPT_URL, PLEX_URL_SOURCE);
    curl_easy_setopt(api_curl, CURLOPT_WRITEFUNCTION,
            dashboard::panel::plex::curl_callback);
    curl_easy_setopt(api_curl, CURLOPT_WRITEDATA, &json_string);
}

plex::~plex(){
    std::cerr << "PLEX DECONSTRUCTOR\n";
    if(_texture != nullptr){
        SDL_DestroyTexture(_texture);
    }
    if(api_curl != nullptr){
        curl_easy_cleanup(api_curl);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Draw function //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void plex::draw(){
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
// Update the information of the obj
// This DOES NOT update the display
void plex::update(){
    std::cerr << "PLEX::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //fetch updates
    //CURL object has been setup in the constructor
    CURLcode res = curl_easy_perform(api_curl);

    //parse the result
    json_doc.Parse(json_string.c_str());

    //update internal state
    //const rapidjson::Value& curr_entry = json_doc["value"];
    
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in the json feed
void plex::update_texture(){
    std::cerr << "PLEX::UPDATE_TEXTURE\n";
    SDL_Rect tgt;
    
    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());




    SDL_SetRenderTarget(board::getRenderer(), NULL);
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
void plex::initTexture(){
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
size_t dashboard::panel::plex::curl_callback(void* contents, size_t size, 
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

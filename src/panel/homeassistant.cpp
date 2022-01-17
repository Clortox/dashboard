///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 14-1-22
// homeassistant panel
//

#include "homeassistant.hpp"
#include "homeassistant_config.hpp"

using namespace dashboard::panel;

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

homeassistant::homeassistant(){
    std::cerr << "HOMEASSISTANT CONSTRUCTOR\n";
    _time_on_screen = HOMEASSISTANT_DEFAULT_ON_SCREEN_TIME;
    _update_interval = std::chrono::milliseconds{HOMEASSISTANT_UPDATE_INTERVAL};
    _texture = nullptr;
    _title = HOMEASSISTANT_TITLE;

    std::string api_string = "Authorization: Bearer ";
    api_string += HOMEASSISTANT_APIKEY;

    headers = NULL;
    headers = curl_slist_append(headers, api_string.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    api_string.clear();

    api_curl = curl_easy_init();
    curl_easy_setopt(api_curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(api_curl, CURLOPT_URL, HOMEASSISTANT_URL);
    curl_easy_setopt(api_curl, CURLOPT_WRITEFUNCTION,
            dashboard::panel::homeassistant::curl_callback);
    curl_easy_setopt(api_curl, CURLOPT_WRITEDATA, &json_string);
    //Write request string in update function
}

homeassistant::~homeassistant(){
    std::cerr << "HOMEASSISTANT DECONSTRUCTOR\n";
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

void homeassistant::draw(){
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
void homeassistant::update(){
    std::cerr << "HOMEASSISTANT::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //perform request
    curl_easy_perform(api_curl);

    std::cerr << json_string << "\n";

    //parse the result
    json_doc.Parse(json_string.c_str());

    //update internal state
    for(rapidjson::SizeType i = 0; i < json_doc.Size(); i++){

        //if is a person entry
        if(strstr(json_doc[i]["entity_id"].GetString(), "person.") != NULL){
            //get the last changed date and convert to a tm
            std::string format_str = json_doc[i]["last_changed"].GetString();
            std::time_t t = datestringToTm(format_str);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&t), "%b %e, %I:%M %p");

            struct home_entry entry = {
                json_doc[i]["attributes"]["friendly_name"].GetString(),
                strcmp(json_doc[i]["state"].GetString(), "home") == 0 ? true : false,
                ss.str(),
            };

            home_entries.push_back(entry);
        }

        //TODO other entries from general home assistant?



    }
    json_string.clear();
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data grabed from json
void homeassistant::update_texture(){
    std::cerr << "HOMEASSISTANT::UPDATE_TEXTURE\n";

    SDL_Rect tgt;

    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());

    constexpr int GAP_SIZE = 10;

    //draw each name and time last seen
    {
        for(size_t i = 0; i < home_entries.size(); ++i){
            tgt.x = (SCREEN_WIDTH / (2 * home_entries.size()))
                + i * (SCREEN_WIDTH / home_entries.size());
            tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
            TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 50} ),
                    home_entries[i].name.c_str(),
                    &tgt.w, &tgt.h);

            tgt.x -= tgt.w / 2;

            SDL_RenderCopy(board::getRenderer(),
                    board::getString(home_entries[i].name,
                    { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
                NULL, &tgt);

            //down a new line
            tgt.y += tgt.h + GAP_SIZE;
            tgt.x += tgt.w / 2;

            std::string home = "";

            //if is home
            if(home_entries[i].home){
                home = "Home";
            } else {
                home = home_entries[i].time;
            }

            TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 50} ),
                home.c_str(),
                &tgt.w, &tgt.h);

            tgt.x -= tgt.w / 2;

            SDL_RenderCopy(board::getRenderer(),
                    board::getString(home,
                    { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
                    NULL, &tgt);
        }
    }


    home_entries.clear();
    SDL_SetRenderTarget(board::getRenderer(), NULL);
}

///////////////////////////////////////
// Lazy load the texture object
// This is to make sure that all of SDL
// is init before we attempt to draw anything
void homeassistant::initTexture(){
    if(_texture == nullptr){
        _texture = SDL_CreateTexture(board::getRenderer(),
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET,
                SCREEN_WIDTH, SCREEN_HEIGHT);

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    }
}

///////////////////////////////////////
// Convert the homeassistant string
// format to a tm object
std::time_t homeassistant::datestringToTm(const std::string& date_string){
    int year = 0, month = 0, day = 0, hour = 0, minuite = 0, second = 0;

    std::tm tm_tmp{};

    if(sscanf(date_string.c_str(), "%4d-%2d-%2dT%2d:%2d:%2d",
                &year, &month, &day, &hour, &minuite, &second) == 6){
        tm_tmp.tm_year = year - 1900;
        tm_tmp.tm_mon = month - 1;
        tm_tmp.tm_mday = day;
        tm_tmp.tm_hour = hour;
        tm_tmp.tm_min = minuite;
        tm_tmp.tm_sec = second;
    }

    return std::mktime(&tm_tmp);
}

///////////////////////////////////////
// Curl callback function
size_t dashboard::panel::homeassistant::curl_callback(void* contents, size_t size,
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

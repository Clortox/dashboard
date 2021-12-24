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
    curl_easy_setopt(api_curl, CURLOPT_URL, PLEX_URL_SOURCE_HISTORY);
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
    curl_easy_setopt(api_curl, CURLOPT_URL, PLEX_URL_SOURCE_HISTORY);
    curl_easy_perform(api_curl);

    //parse the result
    json_doc.Parse(json_string.c_str());

    //update internal state
    rapidjson::Value& curr_entry = json_doc["response"]["data"]["data"];

    for(short i = 0; i < 4; ++i){
        entries.at(i) = {
            truncate(curr_entry[i]["friendly_name"].GetString(), 
                    PLEX_MAX_STRING_LENGTH),
            truncate(curr_entry[i]["ip_address"].GetString(), 
                    PLEX_MAX_STRING_LENGTH),
            truncate(curr_entry[i]["title"].GetString(), 
                    PLEX_MAX_STRING_LENGTH),
            truncate(curr_entry[i]["state"].IsNull() ? "Historical" : "Playing", 
                    PLEX_MAX_STRING_LENGTH),
            };

        std::cerr << entries[i].friendly_name << "\n";
        std::cerr << entries[i].ip_address << "\n";
        std::cerr << entries[i].title << "\n";
        std::cerr << entries[i].state << "\n";
    }

    //get misc entries for right hand bar
    total_plays = std::to_string(json_doc["response"]["data"]["recordsTotal"]
            .GetInt());
    total_plays = truncate("Plays: " + total_plays, PLEX_MAX_STRING_LENGTH);
    total_duration = json_doc["response"]["data"]["total_duration"].GetString();
    total_duration = truncate("Duration: " + total_duration, PLEX_MAX_STRING_LENGTH);

    json_string.clear();

    //make request for top users
    curl_easy_setopt(api_curl, CURLOPT_URL, PLEX_URL_SOURCE_TOP_USERS);
    curl_easy_perform(api_curl);
    json_doc.Parse(json_string.c_str());

    curr_entry = json_doc["response"]["data"]["categories"];

    for(rapidjson::SizeType i = 0; i < curr_entry.Size(); i++){
        top_users[i] = truncate(curr_entry[i].GetString(), PLEX_MAX_STRING_LENGTH);
    }

    json_string.clear();


    //make request for friendly server name
    curl_easy_setopt(api_curl, CURLOPT_URL, PLEX_URL_SOURCE_NAME);
    curl_easy_perform(api_curl);
    json_doc.Parse(json_string.c_str());

    friendly_name = truncate(json_doc["response"]["data"].GetString(), PLEX_MAX_STRING_LENGTH - 10);
    
    json_string.clear();
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in the json feed
void plex::update_texture(){
    std::cerr << "PLEX::UPDATE_TEXTURE\n";
    uint8_t o_red, o_green, o_blue, o_alpha;
    SDL_Rect tgt;

    //save the old colors
    SDL_GetRenderDrawColor(board::getRenderer(), &o_red,
            &o_green, &o_blue, &o_alpha);
    
    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());

    //set the new color
    SDL_SetRenderDrawColor(board::getRenderer(),
            PLEX_BGBOX_RED, PLEX_BGBOX_GREEN, 
            PLEX_BGBOX_BLUE, PLEX_BGBOX_ALPHA);

    //background image
    SDL_RenderCopy(board::getRenderer(),
            board::getImage("plex_background.jpg"), NULL, NULL);

    constexpr int GAP_SIZE = 10;

    //draw the outline rectangles
    {
        tgt.x = GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        tgt.w = (SCREEN_WIDTH / 3) - (2*GAP_SIZE);
        tgt.h = (SCREEN_HEIGHT / 2) - DEF_OVERLAY_BAR_HEIGHT - (2*GAP_SIZE);
        SDL_RenderFillRect(board::getRenderer(), &tgt);

        tgt.x = GAP_SIZE;
        tgt.y = (SCREEN_HEIGHT / 2) + GAP_SIZE;
        tgt.w = (SCREEN_WIDTH / 3) - (2*GAP_SIZE);
        tgt.h = (SCREEN_HEIGHT / 2) - DEF_OVERLAY_BAR_HEIGHT - (2*GAP_SIZE);
        SDL_RenderFillRect(board::getRenderer(), &tgt);

        tgt.x = (SCREEN_WIDTH / 3) + GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        tgt.w = (SCREEN_WIDTH / 3) - (2*GAP_SIZE);
        tgt.h = (SCREEN_HEIGHT / 2) - DEF_OVERLAY_BAR_HEIGHT - (2*GAP_SIZE);
        SDL_RenderFillRect(board::getRenderer(), &tgt);

        tgt.x = (SCREEN_WIDTH / 3) + GAP_SIZE;
        tgt.y = (SCREEN_HEIGHT / 2) + GAP_SIZE;
        tgt.w = (SCREEN_WIDTH / 3) - (2*GAP_SIZE);
        tgt.h = (SCREEN_HEIGHT / 2) - DEF_OVERLAY_BAR_HEIGHT - (2*GAP_SIZE);
        SDL_RenderFillRect(board::getRenderer(), &tgt);

        tgt.x = ((2*SCREEN_WIDTH) / 3) + GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        tgt.w = (SCREEN_WIDTH / 3) - (2*GAP_SIZE);
        tgt.h = (SCREEN_HEIGHT) - (2*DEF_OVERLAY_BAR_HEIGHT) - (2*GAP_SIZE);
        SDL_RenderFillRect(board::getRenderer(), &tgt);
    }

    //draw info for first box
    {
        tgt.x = GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        constexpr int index = 0;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].title.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].title,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].friendly_name.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].friendly_name,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].ip_address.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].ip_address,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].state.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].state,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
    }

    //draw the second box
    {
        tgt.x = GAP_SIZE;
        tgt.y = (SCREEN_HEIGHT / 2) + GAP_SIZE;
        constexpr int index = 1;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].title.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].title,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].friendly_name.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].friendly_name,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].ip_address.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].ip_address,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].state.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].state,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
    }

    //third box
    {
        tgt.x = (SCREEN_WIDTH / 3) + GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        constexpr int index = 2;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].title.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].title,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].friendly_name.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].friendly_name,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].ip_address.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].ip_address,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].state.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].state,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
    }

    //draw info for fourth box
    {
        tgt.x = (SCREEN_WIDTH / 3) + GAP_SIZE;
        tgt.y = (SCREEN_HEIGHT / 2) + GAP_SIZE;
        constexpr int index = 3;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].title.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].title,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].friendly_name.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].friendly_name,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].ip_address.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].ip_address,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28 } ),
                entries[index].state.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(entries[index].state,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28 }),
                NULL, &tgt);
    }

    //draw info for right hand side box
    {
        tgt.x = ((2*SCREEN_WIDTH) / 3) + GAP_SIZE;
        tgt.y = DEF_OVERLAY_BAR_HEIGHT + GAP_SIZE;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 50} ),
                friendly_name.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(friendly_name,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 50}),
                NULL, &tgt);

        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28} ),
                total_plays.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(total_plays,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28}),
                NULL, &tgt);

        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28} ),
                total_duration.c_str(),
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString(total_duration,
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 28}),
                NULL, &tgt);

        //draw the top users
        tgt.y += tgt.h;
        TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 50} ),
                "Top Users",
                &tgt.w, &tgt.h);
        SDL_RenderCopy(board::getRenderer(),
                board::getString("Top Users",
                    {"Roboto_Mono/RobotoMono-Medium.ttf", 50}),
                NULL, &tgt);
        for(unsigned short i = 0; i < top_users.size(); ++i){
            tgt.y += tgt.h;
            TTF_SizeText(board::getFont( {"Roboto_Mono/RobotoMono-Medium.ttf", 28} ),
                    top_users[i].c_str(),
                    &tgt.w, &tgt.h);
            SDL_RenderCopy(board::getRenderer(),
                    board::getString(top_users[i],
                        {"Roboto_Mono/RobotoMono-Medium.ttf", 28}),
                    NULL, &tgt);
        }
    }



    SDL_SetRenderTarget(board::getRenderer(), NULL);

    //reset back to the old render color
    SDL_SetRenderDrawColor(board::getRenderer(),
            o_red, o_green, o_blue, o_alpha);
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
// Helper function to truncate a string with an elipsis
std::string plex::truncate(std::string str, size_t width, bool show_ellipsis){
    if(str.length() > width){
        if(show_ellipsis)
            return str.substr(0, width - 3) + "...";
        else
            return str.substr(0, width);
    }
    return str;
}

///////////////////////////////////////
// Curl callback function
size_t dashboard::panel::plex::curl_callback(void* contents, size_t size, 
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

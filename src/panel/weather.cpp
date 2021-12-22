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
    _update_interval = std::chrono::milliseconds{WEATHER_UPDATE_INTERVAL};
    _texture = nullptr;
    _title = WEATHER_TITLE;
    api_curl = nullptr;
    
    weather_string["clearday"]       = WEATHER_CLEAR_DAY;
    weather_string["clearnight"]     = WEATHER_CLEAR_NIGHT;
    weather_string["pcloudyday"]     = WEATHER_PCLOUDY_DAY;
    weather_string["pcloudynight"]   = WEATHER_PCLOUDY_NIGHT;
    weather_string["mcloudyday"]     = WEATHER_MCLOUDY_DAY;
    weather_string["mcloudynight"]   = WEATHER_MCLOUDY_NIGHT;
    weather_string["cloudyday"]      = WEATHER_CLOUDY_DAY;
    weather_string["cloudynight"]    = WEATHER_CLOUDY_NIGHT;
    weather_string["humidday"]       = WEATHER_HUMID_DAY;
    weather_string["humidnight"]     = WEATHER_HUMID_NIGHT;
    weather_string["lightrainday"]   = WEATHER_LRAIN_DAY;
    weather_string["lightrainnight"] = WEATHER_LRAIN_NIGHT;
    weather_string["rainday"]        = WEATHER_RAIN_DAY;
    weather_string["rainnight"]      = WEATHER_RAIN_NIGHT;
    weather_string["oshowerday"]     = WEATHER_OSHOWER_DAY;
    weather_string["oshowernight"]   = WEATHER_OSHOWER_NIGHT;
    weather_string["ishowerday"]     = WEATHER_ISHOWER_DAY;
    weather_string["ishowernight"]   = WEATHER_ISHOWER_NIGHT;
    weather_string["lightsnowday"]   = WEATHER_LSNOW_DAY;
    weather_string["lightsnownight"] = WEATHER_LSNOW_NIGHT;
    weather_string["snowday"]        = WEATHER_SNOW_DAY;
    weather_string["snownight"]      = WEATHER_SNOW_NIGHT;
    weather_string["rainsnowday"]    = WEATHER_RAINSNOW_DAY;
    weather_string["rainsnownight"]  = WEATHER_RAINSNOW_NIGHT;
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
void weather::update() {
    std::cerr << "WEATHER::UPDATE\n";
    _last_update = std::chrono::high_resolution_clock::now();

    //fetch updates
    //do curl setup and cleanup
    //We cleanup after each operation to save on memory.
    //This is because by default we only fetch every hour
    if(api_curl == nullptr){
        std::cerr << "GRABING WEATHER DATA\n";
        api_curl = curl_easy_init();
        curl_easy_setopt(api_curl, CURLOPT_URL, WEATHER_URL_SOURCE);
        curl_easy_setopt(api_curl, CURLOPT_WRITEFUNCTION,
                dashboard::panel::weather::curl_callback);
        curl_easy_setopt(api_curl, CURLOPT_WRITEDATA, &json_string);
        CURLcode res = curl_easy_perform(api_curl);
        curl_easy_cleanup(api_curl);
        api_curl = nullptr;
    }

    //prase the response
    json_doc.Parse(json_string.c_str());

    //update internal state
    const rapidjson::Value& curr_entry = json_doc["dataseries"];
    //get all entries
    weather_today = &weather_string.at(curr_entry[0]["weather"].GetString());
    weather_tommorow = &weather_string.at(curr_entry[7]["weather"].GetString());
    weather_day_after = &weather_string.at(curr_entry[15]["weather"].GetString());
    temp_today = curr_entry[0]["temp2m"].GetInt();
    temp_tommorow = curr_entry[7]["temp2m"].GetInt();
    temp_day_after = curr_entry[15]["temp2m"].GetInt();


    if(!WEATHER_METRIC){
        temp_today = (temp_today * 1.8) + 32;
        temp_tommorow = (temp_tommorow * 1.8) + 32;
        temp_day_after = (temp_day_after * 1.8) + 32;
    }
}

///////////////////////////////////////
// Update the texture that is being
// displayed, based on data in the json feed
void weather::update_texture(){
    std::cerr << "WEATHER::UPDATE_TEXTURE\n";
    SDL_Rect tgt;
    std::string temp;

    SDL_SetRenderTarget(board::getRenderer(), _texture);
    SDL_RenderClear(board::getRenderer());

    //background image
    SDL_RenderCopy(board::getRenderer(),
            board::getImage("sky.png"), NULL, NULL);

    //current weather
    tgt.y = 50;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            "Currently,",
            &tgt.w, &tgt.h);
    tgt.x = (SCREEN_WIDTH / 2) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString("Currently,",
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            weather_today->first.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (SCREEN_WIDTH / 2) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(weather_today->first.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    temp = std::to_string(temp_today);
    if(WEATHER_METRIC)
        temp += "*C";
    else
        temp += "*F";
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            temp.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (SCREEN_WIDTH / 2) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(temp.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    

    //tommorow's weather
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            "Tommorow,",
            &tgt.w, &tgt.h);
    tgt.y = (3*SCREEN_HEIGHT / 4) - (tgt.h / 2);
    tgt.x = (SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString("Tommorow,",
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            weather_tommorow->first.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(weather_tommorow->first.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    temp = std::to_string(temp_tommorow);
    if(WEATHER_METRIC)
        temp += "*C";
    else
        temp += "*F";
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            temp.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(temp.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);

    //day after's weather
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            "Day after,",
            &tgt.w, &tgt.h);
    tgt.y = (3*SCREEN_HEIGHT / 4) - (tgt.h / 2);
    tgt.x = (5*SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString("Day after,",
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            weather_day_after->first.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (5*SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(weather_day_after->first.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);
    tgt.y += tgt.h;
    temp = std::to_string(temp_day_after);
    if(WEATHER_METRIC)
        temp += "*C";
    else
        temp += "*F";
    TTF_SizeText(board::getFont({ "Roboto_Mono/RobotoMono-Medium.ttf", 50 }),
            temp.c_str(),
            &tgt.w, &tgt.h);
    tgt.x = (5*SCREEN_WIDTH / 6) - (tgt.w / 2);
    SDL_RenderCopy(board::getRenderer(),
            board::getString(temp.c_str(),
                { "Roboto_Mono/RobotoMono-Medium.ttf", 50 }), NULL, &tgt);



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

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
    }
}

///////////////////////////////////////
// Curl callback function
size_t dashboard::panel::weather::curl_callback(void* contents, size_t size, 
        size_t nmemb, void* userp){
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

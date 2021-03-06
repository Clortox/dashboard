///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Board defintions
//

#pragma once

#include "config.hpp"

#include "util/lru.hpp"
#include "panel/panel.hpp"

#include <functional>
#include <unordered_map>
#include <string.h>
#include <chrono>
#include <thread>

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace dashboard {
    struct font_and_size {
        std::string _font;
        size_t _ptsize;

        bool operator==(const font_and_size&) const;
        bool operator!=(const font_and_size&) const;
        bool operator> (const font_and_size&) const;
        bool operator< (const font_and_size&) const;
        bool operator>=(const font_and_size&) const;
        bool operator<=(const font_and_size&) const;
    };

    struct string_and_font {
        std::string _string;
        font_and_size _fs;

        bool operator==(const string_and_font&) const;
        bool operator!=(const string_and_font&) const;
        bool operator> (const string_and_font&) const;
        bool operator< (const string_and_font&) const;
        bool operator>=(const string_and_font&) const;
        bool operator<=(const string_and_font&) const;
    };

    struct font_and_size_hash {
        std::size_t operator()(const font_and_size&) const;
    };

    struct string_and_font_hash {
        std::size_t operator()(const string_and_font&) const;
    };

    typedef std::string texture_path;
    class SDL_Texture_Wrapper {
    public:
        SDL_Texture_Wrapper();
        ~SDL_Texture_Wrapper();
        SDL_Texture_Wrapper(const SDL_Texture_Wrapper&);

        //String loading
        SDL_Texture_Wrapper(const std::string&, const font_and_size&);
        SDL_Texture_Wrapper(const string_and_font&);

        //Image loading
        SDL_Texture_Wrapper(const texture_path&);

        bool load();
        SDL_Texture* texture();

    private:
        SDL_Texture* _texture;
        font_and_size _fs;
        std::string _text;
    };

    typedef std::string font_path;
    class SDL_Font_Wrapper {
    public:
        SDL_Font_Wrapper();
        ~SDL_Font_Wrapper();
        SDL_Font_Wrapper(const SDL_Font_Wrapper&);

        SDL_Font_Wrapper(const font_path&, const size_t);
        SDL_Font_Wrapper(const font_and_size&);

        bool load();
        TTF_Font* font();

    private:
        TTF_Font* _font;
        font_and_size _fs;
    };



    class board {
    public:
        board(const bool = true);
        ~board();

        //sdl setup function
        int init();

        //start main loop
        void start();

        //globals
        inline static SDL_Window*   getWindow();
        inline static SDL_Renderer* getRenderer();

        //memory functions (also globals)
        //these are called by other objects to get their memory
        inline static SDL_Texture* getString(const std::string&, const font_and_size&);
        inline static SDL_Texture* getImage (const std::string&);
        inline static TTF_Font*    getFont  (const font_and_size&);

    private:
        //setup memory management with
        //all const resrouces
        void initConstResources();

        //setup static memory. These are run in initConstResources()
        SDL_Texture* setString(const std::string&, const font_and_size&);
        SDL_Texture* setImage (const std::string&);
        TTF_Font*    setFont  (const font_and_size&);

        SDL_Texture* setString(const string_and_font&, const SDL_Texture_Wrapper&);
        SDL_Texture* setImage (const std::string&, const SDL_Texture_Wrapper&);
        TTF_Font*    setFont  (const font_and_size&, const SDL_Font_Wrapper&);

        //containers for resources
        inline static std::unordered_map<std::string, SDL_Texture_Wrapper> _textures;
        inline static std::unordered_map<font_and_size, 
            SDL_Font_Wrapper, font_and_size_hash> _fonts;
        inline static std::unordered_map<string_and_font, SDL_Texture_Wrapper, 
            string_and_font_hash> _strings;

        inline static clortox::LRUCache<string_and_font, 
            SDL_Texture_Wrapper, string_and_font_hash> _dynamic_strings;
        
        //TODO: Dynamic images?
        //TODO: Dynamic Fonts?
        

        //local pointers to the globals
        inline static SDL_Window* _window;
        inline static SDL_Renderer* _renderer;
    };
}

///////////////////////////////////////////////////////////////////////////////
// Static functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Window* dashboard::board::getWindow(){
    if(_window == nullptr){
        _window = SDL_CreateWindow(WINDOW_TITLE,
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT,
                SDL_WINDOW_FLAGS);

        if(_window == NULL)
            SDL_Log("Window could not be created, %s\n", SDL_GetError());
    }
    return _window;
}

SDL_Renderer* dashboard::board::getRenderer(){
    if(_renderer == nullptr){
        _renderer = SDL_CreateRenderer(board::getWindow(), -1,
                SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        if(_renderer == NULL)
            SDL_Log("Renderer could not be created, %s\n", SDL_GetError());
    }

    return _renderer;
}

SDL_Texture* dashboard::board::getString(const std::string& text, const font_and_size& fs){
    //check for string in static strings, then check in dynamic strings

    string_and_font sf = {text, fs};
    if(_strings.find(sf) != _strings.end())
        return _strings.find(sf)->second.texture();
    
    //is dynamic string, generate:
    return _dynamic_strings.get(sf).texture();
}

SDL_Texture* dashboard::board::getImage(const std::string& path){
    if(_textures.find(path) != _textures.end())
        return _textures.find(path)->second.texture();

    //TODO: Dynamic Images?
    //not found, return null
    return nullptr;
}

TTF_Font* dashboard::board::getFont(const font_and_size& fs){
    if(_fonts.find(fs) != _fonts.end()){
        return _fonts.find(fs)->second.font();
    }

    //TODO: Dynamic Fonts? Is the needed?
    //not found, return null
    return nullptr;
}

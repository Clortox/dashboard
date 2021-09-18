///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Board defintions
//

#pragma once

#include "config.hpp"

#include "util/lru.hpp"

#include <functional>
#include <unordered_map>

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

        SDL_Texture_Wrapper(const std::string&, const font_and_size&);
        SDL_Texture_Wrapper(const string_and_font&);

        SDL_Texture_Wrapper(const texture_path&);

        SDL_Texture* texture() const;

    private:
        SDL_Texture* _texture;
    };

    typedef std::string font_path;
    class SDL_Font_Wrapper {
    public:
        SDL_Font_Wrapper();
        ~SDL_Font_Wrapper();

        SDL_Font_Wrapper(const font_path&, const size_t);
        SDL_Font_Wrapper(const font_and_size&);

        TTF_Font* font() const;

    private:
        TTF_Font* _font;
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

        //containers for resources
        inline static std::unordered_map<std::string, SDL_Texture_Wrapper> _textures;
        inline static std::unordered_map<font_and_size, 
            SDL_Font_Wrapper, font_and_size_hash> _fonts;
        inline static std::unordered_map<string_and_font, SDL_Texture_Wrapper, 
            string_and_font_hash> _strings;

        static clortox::LRUCache<string_and_font, 
            SDL_Texture_Wrapper, string_and_font_hash> _dynamic_strings;

        //TODO: Dynamic images?
        //TODO: Dynamic Fonts?

        //local pointers to the globals
        SDL_Window* _window;
        SDL_Renderer* _renderer;
    };
}

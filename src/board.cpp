///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Board implementation
//

#include "board.hpp"

using namespace dashboard;

///////////////////////////////////////////////////////////////////////////////
// Structs ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

size_t font_and_size_hash::operator()(const font_and_size& value) const{
    size_t hlhs = std::hash<std::string>{}(value._font);
    size_t hrhs = std::hash<size_t>{}(value._ptsize);

    hlhs ^= hrhs + 0x9e3779b9 + (hlhs << 6) + (hlhs >> 2);
    return hlhs;
}

size_t string_and_font_hash::operator()(const string_and_font& value) const {
    size_t hlhs = std::hash<std::string>{}(value._string);
    size_t hrhs = font_and_size_hash{}(value._fs);

    hlhs ^= hrhs + 0x9e3779b9 + (hlhs << 6) + (hlhs >> 2);
    return hlhs;
}

bool font_and_size::operator==(const font_and_size& rhs) const{
    if(this->_font == rhs._font && this->_ptsize == rhs._ptsize)
        return true;
    return false;
}
bool font_and_size::operator!=(const font_and_size& rhs) const{
    if(this->_font == rhs._font && this->_ptsize == rhs._ptsize)
        return false;
    return true;
}
bool font_and_size::operator> (const font_and_size& rhs) const{
    if(this->_font > rhs._font)
        return true;
    return false;
}
bool font_and_size::operator< (const font_and_size& rhs) const{
    if(this->_font < rhs._font)
        return false;
    return true;
}
bool font_and_size::operator>=(const font_and_size& rhs) const{
    return !(*this < rhs);
}
bool font_and_size::operator<=(const font_and_size& rhs) const{
    return !(*this > rhs);
}

bool string_and_font::operator==(const string_and_font& rhs) const{
    if(this->_string == rhs._string && this->_fs == rhs._fs)
        return true;
    return false;
}
bool string_and_font::operator!=(const string_and_font& rhs) const{
    if(this->_string == rhs._string && this->_fs == rhs._fs)
        return false;
    return true;
}
bool string_and_font::operator> (const string_and_font& rhs) const{
    if(this->_string > rhs._string)
        return true;
    return false;
}
bool string_and_font::operator< (const string_and_font& rhs) const{
    if(this->_string < rhs._string)
        return false;
    return true;
}
bool string_and_font::operator>=(const string_and_font& rhs) const{
    return !(*this < rhs);
}
bool string_and_font::operator<=(const string_and_font& rhs) const{
    return !(*this > rhs);
}

///////////////////////////////////////////////////////////////////////////////
// SDL_Texture_Wrapper ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Texture_Wrapper::SDL_Texture_Wrapper(){
    _texture = nullptr;
}

SDL_Texture_Wrapper::~SDL_Texture_Wrapper(){
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
}

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const std::string& text, const font_and_size& fs){
    SDL_Surface* tmpSurface;

    TTF_Font* fs_font = board::getFont(fs);

    tmpSurface = TTF_RenderText_Solid(fs_font, text.c_str(), 
            { BOARD_RED, BOARD_GREEN, BOARD_BLUE });

    if(tmpSurface == NULL){
        SDL_Log("SDL_Texture_Wrapper: Failed to create surface from string (%s): %s\n",
                text.c_str(), SDL_GetError());
        _texture = NULL;
        return;
    }

    _texture = SDL_CreateTextureFromSurface(board::getRenderer(), tmpSurface);

    if(_texture == NULL){
        SDL_Log("SDL_Texture_Wrapper: failed to create texture from surface of string (%s): %s\n",
                text.c_str(), SDL_GetError());
        SDL_FreeSurface(tmpSurface);
        return;
    }

    //texture was created sucessfully, cleanup surface

    SDL_FreeSurface(tmpSurface);
    SDL_UnlockTexture(_texture);
}

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const string_and_font& sf) 
    : SDL_Texture_Wrapper(sf._string, sf._fs) {};

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const texture_path& path){
    SDL_Surface* tmpSurface;

    tmpSurface = IMG_Load(path.c_str());

    if(tmpSurface == NULL){
        SDL_Log("SDL_Texture_Wrapper: failed to load image (%s): %s\n",
                path.c_str(), SDL_GetError());
        return;
    }

    _texture = SDL_CreateTextureFromSurface(board::getRenderer(), tmpSurface);

    if(_texture == NULL){
        SDL_Log("SDL_Texture_Wrapper: failed to create texture from surface of image (%s): %s\n",
                path.c_str(), SDL_GetError());
        SDL_FreeSurface(tmpSurface);
        return;
    }

    //texture was created sucessfully, cleanup surface
        
    SDL_FreeSurface(tmpSurface);
    SDL_UnlockTexture(_texture);
}

SDL_Texture* SDL_Texture_Wrapper::texture() const {
    return _texture;
}

///////////////////////////////////////////////////////////////////////////////
// SDL_Font_Wrapper ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Font_Wrapper::SDL_Font_Wrapper(){
    _font = nullptr;
}

SDL_Font_Wrapper::~SDL_Font_Wrapper(){
    if(_font != nullptr)
        TTF_CloseFont(_font);
}

SDL_Font_Wrapper::SDL_Font_Wrapper(const font_path& fp, const size_t ptsize){
    _font = TTF_OpenFont(fp.c_str(), ptsize);
    
    if(_font == NULL){
        SDL_Log("SDL_Font_Wrapper: Failed to open font (%s) with ptsize (%lu): %s\n",
                fp.c_str(), ptsize, SDL_GetError());
        return;
    }
}

SDL_Font_Wrapper::SDL_Font_Wrapper(const font_and_size& fs){
    _font = TTF_OpenFont(fs._font.c_str(), fs._ptsize);
    
    if(_font == NULL){
        SDL_Log("SDL_Font_Wrapper: Failed to open font (%s) with ptsize (%lu): %s\n",
                fs._font.c_str(), fs._ptsize, SDL_GetError());
        return;
    }
}

TTF_Font* SDL_Font_Wrapper::font() const {
    return _font;
}

///////////////////////////////////////////////////////////////////////////////
// Static functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Window* board::getWindow(){
    static SDL_Window* _window = nullptr;

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

SDL_Renderer* board::getRenderer(){
    static SDL_Renderer* _renderer = nullptr;

    if(_renderer == nullptr){
        _renderer = SDL_CreateRenderer(board::getWindow(), -1,
                SDL_RENDERER_ACCELERATED);

        if(_renderer == NULL)
            SDL_Log("Renderer could not be created, %s\n", SDL_GetError());
    }

    return _renderer;
}

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

board::board(const bool init = true){
    _window = nullptr;
    _renderer = nullptr;

    if(init)
        this->init();
}

board::~board(){
    if(_renderer != nullptr)
        SDL_DestroyRenderer(_renderer); 

    if(_window != nullptr)
        SDL_DestroyWindow(_window);

    //allocated resources will be taken care of by the LRUCache

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int board::init(){
    SDL_Log("Init SDL...\n");

    //check if already has been init
    if(_window != NULL || _renderer != NULL){
        SDL_Log("Failed to init SDL: Already init\n");
        return 1;
    }

    //setup sdl
    if(SDL_Init(SDL_FLAGS) != 0){
        SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
        return -1;
    }

    //create window
    _window = board::getWindow();
    if(_window == NULL){
        SDL_Log("Failed to init SDL; Window could not be created: %s\n",
                SDL_GetError());
        return -2;
    }

    //create renderer
    _renderer = board::getRenderer();
    if(_renderer == NULL){
        SDL_Log("Failed to init SDL; Renderer could not be created: %s\n",
                SDL_GetError());
        return -3;
    }

    //set cursor mode
    SDL_ShowCursor(SDL_SHOW_CURSOR);


    //enable alpha
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    //init sdl_image
    if((IMG_Init(IMG_FLAGS) & IMG_FLAGS) != IMG_FLAGS){
        SDL_Log("Failed to init SDL; Failed to init SDL_Image: %s\n",
                IMG_GetError());
        return -4;
    }

    //init sdl_ttf
    if(TTF_Init() == -1){
        SDL_Log("Failed to init SDL; Failed to init SDL_TTF: %s\n",
                TTF_GetError());
        return -5;
    }

    SDL_Log("Sucessfully setup SDL\n");
    return 0;
}

///////////////////////////////////////
// This is called to start the main loop.
// This is where most of the logic lives
void board::start(){




}

///////////////////////////////////////////////////////////////////////////////
// Memory Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Texture* board::getString(const std::string& text, const font_and_size& fs){
    //check for string in static strings, then check in dynamic strings

    string_and_font sf = {text, fs};
    if(_strings.find(sf) != _strings.end())
        return _strings.find(sf)->second.texture();
    
    //is dynamic string, generate:
    return _dynamic_strings.get(sf).texture();
}

SDL_Texture* board::getImage(const std::string& path){
    if(_textures.find(path) != _textures.end())
        return _textures.find(path)->second.texture();

    //TODO: Dynamic Images?
    //not found, return null
    return nullptr;
}

TTF_Font* board::getFont(const font_and_size& fs){
    if(_fonts.find(fs) != _fonts.end())
        return _fonts.find(fs)->second.font();

    //Dynamic Fonts? Is the needed?
    //not found, return null
    return nullptr;
}

SDL_Texture* board::setString(const std::string& text, const font_and_size& fs){
    string_and_font sf = {text, fs};
    _strings.insert_or_assign(sf, SDL_Texture_Wrapper(text, fs));
    return _strings[sf].texture();
}

SDL_Texture* board::setImage(const std::string& path){
    _textures.insert_or_assign(path, SDL_Texture_Wrapper(path));
    return _textures[path].texture();
}

TTF_Font* board::setFont(const font_and_size& fs){
    _fonts.insert_or_assign(fs, SDL_Font_Wrapper(fs));
    return _fonts[fs].font();
}

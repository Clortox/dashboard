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
    _fs = { "", 0 };
    _text = "";
}

SDL_Texture_Wrapper::~SDL_Texture_Wrapper(){
    if(_texture != nullptr)
        SDL_DestroyTexture(_texture);
}

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const SDL_Texture_Wrapper& rhs){
    _texture = nullptr;
    _fs = rhs._fs;
    _text = rhs._text;
}

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const std::string& text, 
        const font_and_size& fs) : _fs(fs), _text(text){
    _texture = nullptr;
}

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const string_and_font& sf) 
    : SDL_Texture_Wrapper(sf._string, sf._fs) {};

SDL_Texture_Wrapper::SDL_Texture_Wrapper(const texture_path& path)
    : _text(path) {
    _texture = nullptr;
}

bool SDL_Texture_Wrapper::load(){
    //if the texture is already loaded, we dont need to do anything
    if(_texture != nullptr)
        return true;

    SDL_Surface* tmpSurface = NULL;

    if((_fs._font == "" || _fs._ptsize == 0) && !_text.empty()){ //load a texture
        tmpSurface = IMG_Load(_text.c_str());

        if(tmpSurface == NULL){
            SDL_Log("SDL_Texture_Wrapper::load() : failed to load image (%s): %s\n",
                    _text.c_str(), SDL_GetError());
            return false;
        }

        _texture = SDL_CreateTextureFromSurface(board::getRenderer(), tmpSurface);

        if(_texture == NULL){
            SDL_Log("SDL_Texture_Wrapper: failed to create texture from surface \
                    of image (%s): %s\n",
                    _text.c_str(), SDL_GetError());
            SDL_FreeSurface(tmpSurface);
            return false;
        }

        //texture was created sucessfully, cleanup surface
            
        SDL_FreeSurface(tmpSurface);
        SDL_UnlockTexture(_texture);
        return true;

    } else if (_fs._font != "" && _fs._ptsize != 0){ //if _fs is in a valid state
        //load a string
        TTF_Font* fs_font = board::getFont(_fs);

        if(fs_font == nullptr){
            SDL_Log("SDL_Texture_Wrapper::load() : Failed to find given font (%s %lu)\n",
                    _fs._font.c_str(), _fs._ptsize);
            return false;
        }

        tmpSurface = TTF_RenderText_Solid(fs_font, _text.c_str(), 
                { BOARD_RED, BOARD_GREEN, BOARD_BLUE });

        if(tmpSurface == NULL){
            SDL_Log("SDL_Texture_Wrapper: Failed to create surface from string (%s): %s\n",
                    _text.c_str(), SDL_GetError());
            return false;
        }

        _texture = SDL_CreateTextureFromSurface(board::getRenderer(), tmpSurface);

        if(_texture == NULL){
            SDL_Log("SDL_Texture_Wrapper: failed to create texture from surface of \
                    string (%s): %s\n",
                    _text.c_str(), SDL_GetError());
            SDL_FreeSurface(tmpSurface);
            return false;
        }

        //texture was created sucessfully, cleanup surface

        SDL_FreeSurface(tmpSurface);
        SDL_UnlockTexture(_texture);

        return true;

    } else { //not a valid state, return nothing
        SDL_Log("SDL_Texture_Wrapper::load() : In an invalid state! Cannot load anything! \
                (%s %lu) (%s)\n", _fs._font.c_str(), _fs._ptsize, _text.c_str());
        return false;
    }
}

SDL_Texture* SDL_Texture_Wrapper::texture() {
    if(_texture == nullptr)
        load();
    return _texture;
}

///////////////////////////////////////////////////////////////////////////////
// SDL_Font_Wrapper ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Font_Wrapper::SDL_Font_Wrapper(){
    _font = nullptr;
    _fs = { "", 0 };
}

SDL_Font_Wrapper::~SDL_Font_Wrapper(){
    if(_font != nullptr)
        TTF_CloseFont(_font);
}

SDL_Font_Wrapper::SDL_Font_Wrapper(const SDL_Font_Wrapper& rhs){
    _font = nullptr;
    _fs = rhs._fs;
}

SDL_Font_Wrapper::SDL_Font_Wrapper(const font_path& fp, const size_t ptsize){
    _font = nullptr;
    _fs._font = fp;
    _fs._ptsize = ptsize;
}

SDL_Font_Wrapper::SDL_Font_Wrapper(const font_and_size& fs)
    : _fs(fs) {
    _font = nullptr;
}

bool SDL_Font_Wrapper::load(){
    _font = TTF_OpenFont(_fs._font.c_str(), _fs._ptsize);
    
    if(_font == NULL){
        SDL_Log("SDL_Font_Wrapper: Failed to open font (%s) with ptsize (%lu): %s\n",
                _fs._font.c_str(), _fs._ptsize, SDL_GetError());
        return false;
    }
    
    return true;
}

TTF_Font* SDL_Font_Wrapper::font() {
    if(_font == nullptr)
        load();
    return _font;
}

///////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

board::board(const bool init){
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

///////////////////////////////////////////////////////////////////////////////
// Board init functions ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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
    initConstResources();
    //get Panel size
    const size_t panel_count = sizeof(PANELS) / sizeof(PANELS[0]);
    size_t i = 0;

    //timing, for dealing with framerates
    auto start = std::chrono::high_resolution_clock::now();
    static const std::chrono::nanoseconds s{1000000000};

    //frame counter
    static unsigned long long int fcount = 0;

    SDL_Log("Starting main loop...\n");
    for(;;){
        start = std::chrono::high_resolution_clock::now();
        fcount++;

        //check if we can loop back over
        if(i > panel_count)
            i = 0;

        //SDL_RenderClear(_renderer);

        //PLACEHOLDER, cycle color
        {
            static uint8_t red = 0;
            static bool up = true;

            SDL_SetRenderDrawColor(_renderer, red, 
                    BOARD_GREEN, BOARD_BLUE, SDL_ALPHA_OPAQUE);

            SDL_RenderClear(_renderer);

            if(up){
                if(red == 254)
                    up = false;
                red++;
            } else {
                if(red == 1)
                    up = true;
                red --;
            }
        }
        //END PLACEHOLDER


        //call draw on the current panel
        PANELS[i]->draw();

        if(fcount % 10 == 0)
            std::cerr << "Frame : " << fcount << "\n";

        SDL_RenderPresent(_renderer);


        //wait for frame
        std::this_thread::sleep_for((s / MAX_FRAMERATE) -
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now() - start));
    }
}

///////////////////////////////////////
// Grab all constant resources from 
// the configuration file, as well as
// all panel's configurations
void board::initConstResources(){
    //load all resrouces declared in config.hpp
    std::string fullPath = DATA_DIR;
    SDL_Log("Allocating all static resources...\n");
    SDL_Log("Base path: %s", fullPath.c_str());
    std::cerr << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";

    //load images into memory
    fullPath = DATA_IMG;
    fullPath += "/";
    SDL_Log("Static images directory prefix: %s\n", fullPath.c_str());
    SDL_Log("Loading static images into working memory...\n");
    for(unsigned int i = 0; 
            i < (sizeof(IMAGE_LOCATIONS)/sizeof(IMAGE_LOCATIONS[0])); ++i){

        SDL_Texture_Wrapper tw(fullPath + IMAGE_LOCATIONS[0]);

        SDL_Log("Loaded image %s at memory location %p\n",
                IMAGE_LOCATIONS[i],
                setImage(IMAGE_LOCATIONS[0],
                    tw));
    }
    SDL_Log("Loaded static images into working memory\n");
    std::cerr << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";

    //load fonts into memory
    fullPath = DATA_FONT;
    fullPath += "/";
    SDL_Log("Static Fonts directory prefix: %s\n", fullPath.c_str());
    SDL_Log("Loading fonts into working memory...\n");
    for(unsigned int i = 0;
            i < (sizeof(FONT_LOCATIONS)/sizeof(FONT_LOCATIONS[0])); ++i){

        SDL_Font_Wrapper fw(fullPath + FONT_LOCATIONS[0]._name, 
                FONT_LOCATIONS[0]._size);

        SDL_Log("Loaded font %s %lu at memory location %p\n",
                FONT_LOCATIONS[i]._name,
                FONT_LOCATIONS[i]._size,
                setFont({FONT_LOCATIONS[i]._name, FONT_LOCATIONS[i]._size},
                    fw));
    }
    SDL_Log("Loaded fonts into working memory\n");
    std::cerr << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
    SDL_Log("Loading static strings into working memory...\n");
    for(unsigned int i = 0;
            i < (sizeof(CONST_STRINGS)/sizeof(CONST_STRINGS[0])); ++i){

        SDL_Texture_Wrapper tw(CONST_STRINGS[i]._text,
                { CONST_STRINGS[i]._font_size._name,
                  CONST_STRINGS[i]._font_size._size });

        SDL_Log("Loaded string (\"%s\") with font %s %lu at memory location %p\n",
                CONST_STRINGS[i]._text,
                CONST_STRINGS[i]._font_size._name,
                CONST_STRINGS[i]._font_size._size,
                setString({ CONST_STRINGS[i]._text,
                    { CONST_STRINGS[i]._font_size._name,
                      CONST_STRINGS[i]._font_size._size }},
                      tw));
    }
    SDL_Log("Loaded static strings into working memory\n");
    std::cerr << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n";
}

///////////////////////////////////////////////////////////////////////////////
// Memory Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SDL_Texture* board::setString(const std::string& text, const font_and_size& fs){
    string_and_font sf = {text, fs};
    _strings.insert_or_assign(sf, SDL_Texture_Wrapper(text, fs));
    return _strings[sf].texture();
}

SDL_Texture* board::setImage(const std::string& name){
    _textures.insert_or_assign(name, SDL_Texture_Wrapper(name));
    return _textures[name].texture();
}

TTF_Font* board::setFont(const font_and_size& fs){
    _fonts.insert_or_assign(fs, SDL_Font_Wrapper(fs));
    return _fonts[fs].font();
}

SDL_Texture* board::setString(const string_and_font& sf, const SDL_Texture_Wrapper& tw){
    _strings.insert_or_assign(sf, tw);
    return _strings[sf].texture();
}

SDL_Texture* board::setImage(const std::string& name, const SDL_Texture_Wrapper& tw){
    _textures.insert_or_assign(name, tw);
    return _textures[name].texture();
}

TTF_Font* board::setFont(const font_and_size& fs, const SDL_Font_Wrapper& fw){
    _fonts.insert_or_assign(fs, fw);
    return _fonts[fs].font();
}

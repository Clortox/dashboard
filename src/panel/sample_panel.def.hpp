///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 22-12-21
// Sample panel hpp
//

#pragma once

#include "panel.hpp"

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <chrono>

namespace dashboard::panel {
    class sample_panel : public panel {
    public:
        sample_panel();
        ~sample_panel();

        //This will be called every frame your panel is selected as the active
        //panel
        void draw();
    private:
        //This is a sample value for this sample panel. You can delete this
        int i;

        //This is for you to update the data for your panel. You can think of
        //this as the Model component of an MVC
        void update();
        //This is for you to update the texture for your panel. You can think
        //of this as the View component of an MVC
        void update_texture();
        //This is for you to setup the _texture value. You should set it up
        //here and not the contructor, as there is no gaurentee that SDL will
        //be setup when the constructor is called
        void initTexture();

        //this is the last time that the data was updated. By default this is
        //updated to the current time when update() is called
        std::chrono::time_point<std::chrono::high_resolution_clock> _last_update;
        //this is how long we should wait between updating, in milliseconds
        std::chrono::milliseconds _update_interval;
    };

}

#include "../board.hpp"

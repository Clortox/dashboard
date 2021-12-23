///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// pannel implementation
//

#include "panel.hpp"

using namespace dashboard::panel; 

void panel::forceUpdate(){
    if(_texture == nullptr)
        initTexture();

    update();
    update_texture();
}

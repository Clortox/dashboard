///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Handlers definitions
//

#pragma once

#include <csignal>
#include <iostream>

namespace dashboard {
    namespace handlers {
        void setHandlers();
        
        void intHandler(int);
    }
}

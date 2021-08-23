///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Handlers implementation
//

#include "handler.hpp"

namespace dashboard {
    namespace handlers {
        void intHandler(int dummy){
            std::cerr << "Received, ctrl+c, quitting!\n";
            exit(0);
        }

        void setHandlers(){
            //go through and set every handler
            std::signal(SIGINT, intHandler);
        }
    }
}



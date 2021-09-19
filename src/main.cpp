///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Entry point
//

#include "config.hpp"
#include "handler/handler.hpp"
#include "board.hpp"

#include <iostream>

int main(int argc, char** argv){
    dashboard::handlers::setHandlers();

    dashboard::board _board(false);

    if(_board.init() != 0){
        std::cerr << "Due to errors, " << argv[0] 
            << " was unable to start, quitting!" << std::endl;
        return -1;
    }

    _board.start();

    return 0;
}

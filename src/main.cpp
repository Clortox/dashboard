///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// Entry point
//

#include "config.hpp"
#include "handler/handler.hpp"
#include "board.hpp"

int main(int argc, char** argv){
    dashboard::handlers::setHandlers();

    dashboard::board _board;

    _board.start();


    return 0;
}

#include "gm2d3_util.h"
#include "../cmake/gm2d3_cmake.h"

#include <iostream>
#include <string>

int gDEBUG_LEVEL = 0;

void
debug_print(int level_guard, std::string msg)
{
    if (level_guard <= gDEBUG_LEVEL)
    {
        std::cout << "[GM2D3::v" << GM2D3_VERSION_MAJOR << "." << GM2D3_VERSION_MINOR << "] " << msg << std::endl;
    }
}



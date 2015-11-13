#include "gm2d3_util.h"

#include <iostream>

void
debug_print(int level_guard, std::string msg)
{
    if (level_guard <= gDEBUG_LEVEL) { std::cout << msg; }
}

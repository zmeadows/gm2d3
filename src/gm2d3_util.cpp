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
        std::cout << "[GM2D3::v" << GM2D3_VERSION_MAJOR << "." << GM2D3_VERSION_MINOR << "] "
            << msg << std::endl;
    }
}

std::string
axis_to_string(Axis axis)
{
    switch (axis)
    {
        case Axis::AZIMUTHAL:
            return "AZIMUTHAL";
            break;
        case Axis::VERTICAL:
            return "VERTICAL";
            break;
        case Axis::RADIAL:
            return "RADIAL";
            break;
    }
}

Fl_Color LIGHT_RED() { return fl_rgb_color(186,60,60); }
Fl_Color DARK_RED() { return fl_rgb_color(139,39,39); }

Fl_Color LIGHT_GREEN() { return fl_rgb_color(110,206,110); }
Fl_Color DARK_GREEN() { return fl_rgb_color(20,114,20); }

Fl_Color LIGHT_BLUE() { return fl_rgb_color(104,127,240); }
Fl_Color DARK_BLUE() { return fl_rgb_color(41,63,174); }

Fl_Color YELLOW() { return fl_rgb_color(233,233,123); }

void const
print_logo()
{
    std::cout 
        << termcolor::red << R"( 
   ___   __  __    ___     ___     ____  
  / __| |  \/  |  |_  )   |   \   |__ /  )"
        << termcolor::green << R"( 
 | (_ | | |\/| |   / /    | |) |   |_ \  
  \___| |_|__|_|  /___|   |___/   |___/  )"
        << termcolor::blue << R"( 
_|"""""|_|"""""|_|"""""|_|"""""|_|"""""| 
"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-'"`-0-0-' )"
    << termcolor::reset << std::endl;
}

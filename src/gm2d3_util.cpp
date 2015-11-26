#include "gm2d3_util.h"
#include "../cmake/gm2d3_cmake.h"

#include <iostream>
#include <string>

int gDEBUG_LEVEL = 0;

void
debug_print(int level_guard, DebugStatementType type, std::string msg)
{
    std::string prefix = 
        "[GM2D3::v" 
        + std::to_string(GM2D3_VERSION_MAJOR) 
        + "." + std::to_string(GM2D3_VERSION_MINOR) 
        + "] ";

    if (level_guard <= gDEBUG_LEVEL)
    {
        switch (type)
        {
            case DebugStatementType::SUCCESS:
                std::cout << termcolor::green << prefix << termcolor::reset;
                break;
            case DebugStatementType::WARNING:
                std::cout << termcolor::yellow << prefix << termcolor::reset;
                break;
            case DebugStatementType::ERROR:
                std::cout << termcolor::red << prefix << termcolor::reset;
                break;
        }

        std::cout << msg << std::endl;
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

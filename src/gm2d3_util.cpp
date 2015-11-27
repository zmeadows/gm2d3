#include "gm2d3_util.h"
#include "../cmake/gm2d3_cmake.h"

#include <iostream>
#include <string>
#include <time.h>
#include <mutex>

int gDEBUG_LEVEL = 0;

void
debug_print(int level_guard, DebugStatementType type, std::string msg)
{
    std::lock_guard<std::mutex> guard(print_mutex);
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
            case DebugStatementType::GENERIC:
                std::cout << prefix << termcolor::reset;
                break;
        }


        time_t raw_time;
        time(&raw_time);

        std::string msg_time = std::string(asctime(localtime(&raw_time)));
        msg_time.pop_back(); // remove newline at end

        std::cout << "[" << msg_time << "] " << msg << std::endl;

    }

}

void
debug_print(const GM2D3Exception &gex)
{
    DebugStatementType d;

    switch (gex.type)
    {
        case GM2D3Exception::Type::Programmer:
            d = DebugStatementType::ERROR;
            break;
        case GM2D3Exception::Type::Config:
            d = DebugStatementType::ERROR;
            break;
        case GM2D3Exception::Type::SafetyWarning:
            d = DebugStatementType::WARNING;
            break;
    }

    std::string full_msg = gex.msg;
    full_msg += "\n\tFILE: " + std::string(gex.filename);
    full_msg += "\n\tLINE: " + std::to_string(gex.linum);

    debug_print(0, d, full_msg);
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

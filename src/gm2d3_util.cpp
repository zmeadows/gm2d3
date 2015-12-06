#include "gm2d3_util.h"
#include "../cmake/gm2d3_cmake.h"

#include <iostream>
#include <string>
#include <time.h>
#include <mutex>
#include <vector>

int gDEBUG_LEVEL = 0;

GM2D3DebugPrinter *GM2D3DebugPrinter::static_instance = nullptr;

void
GM2D3DebugPrinter::print()
{
    std::string prefix =
        "[GM2D3::v"
        + std::to_string(GM2D3_VERSION_MAJOR)
        + "." + std::to_string(GM2D3_VERSION_MINOR)
        + "] ";

    switch (type_)
    {
    case DebugStatementType::ATTEMPT:
        std::cout << prefix;
        break;
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

    std::cout << "[" << msg_time << "] " << message_ << std::endl;

    while (!sub_messages_.empty())
    {
        std::cout << "\t" << sub_messages_.back() << std::endl;
        sub_messages_.pop_back();
    }

    if (type_ == DebugStatementType::SUCCESS) std::cout << std::endl;
}

void
debug_print(const unsigned level_guard, const DebugStatementType type,
            const std::string &message, const std::vector<std::string> &sub_messages)
{

    if (level_guard > gDEBUG_LEVEL)
    {
        return;
    }
    else
    {
        GM2D3DebugPrinter::instance()->lock();

        GM2D3DebugPrinter::instance()->set_type(type);
        GM2D3DebugPrinter::instance()->set_message(message);
        GM2D3DebugPrinter::instance()->set_sub_messages(sub_messages);

        GM2D3DebugPrinter::instance()->print();
        GM2D3DebugPrinter::instance()->unlock();

    }

}

void
debug_print(const unsigned level_guard, const DebugStatementType type,
            const std::string &message)
{
    std::vector<std::string> empty;
    debug_print(level_guard, type, message, empty);
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
    std::vector<std::string> sub_msgs;
    sub_msgs.push_back("FILE: " + std::string(gex.filename));
    sub_msgs.push_back("LINE: " + std::to_string(gex.linum));

    debug_print(0, d, gex.msg, sub_msgs);
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

std::string
controller_type_to_string(ControllerType ct)
{
    switch (ct)
    {
        case ControllerType::Fake:
            return "Fake";
            break;
        case ControllerType::RaspberryPi:
            return "Raspberry Pi";
            break;
        case ControllerType::Galil:
            return "Galil";
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

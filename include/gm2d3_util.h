#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <string>
#include <memory>

#include "termcolor.hpp"

#include "gm2d3_const.h"

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;
static std::mutex print_mutex;

std::string axis_to_string(Axis axis);

void const print_logo();

class GM2D3Exception {
    public:

        enum class Type {
            Programmer,
            Config,
            SafetyWarning
        };

        GM2D3Exception(GM2D3Exception::Type _type, std::string _msg, unsigned int _linum, std::string _filename)
            : type(_type), msg(_msg), linum(_linum), filename(_filename) {}

        const GM2D3Exception::Type type;
        const std::string msg;
        const unsigned int linum;
        const std::string filename;
};

#define make_gm2d3_exception(t,m) GM2D3Exception(t,m,__LINE__,__FILE__)

enum class DebugStatementType {
    ATTEMPT,
    SUCCESS,
    WARNING,
    ERROR,
    GENERIC
};

class GM2D3DebugPrinter {
    public:
        GM2D3DebugPrinter(void) : message_(""), level_guard_(0), type_(DebugStatementType::GENERIC) {}

        static GM2D3DebugPrinter *static_instance;

        static GM2D3DebugPrinter* instance()
        {
            if (!static_instance)
                static_instance = new GM2D3DebugPrinter();
            return static_instance;
        }

        void lock(void) { print_mutex.lock(); }
        void unlock(void) { print_mutex.unlock(); }
        void print() const;

        void set_message(std::string message) { message_ = message; }
        void set_level_guard(unsigned int level_guard) { level_guard_ = level_guard; }
        void set_type(DebugStatementType type) { type_ = type; }

    private:
        std::mutex print_mutex_;
        std::string message_;
        unsigned int level_guard_;
        DebugStatementType type_;
};

void debug_print(int level_guard, DebugStatementType type, std::string msg);
void debug_print(const GM2D3Exception &gex);

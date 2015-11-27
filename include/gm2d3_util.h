#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <string>

#include "termcolor.hpp"

#include "gm2d3_const.h"

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;
static std::mutex print_mutex;

enum class DebugStatementType {
    SUCCESS,
    WARNING,
    ERROR
};


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

void debug_print(int level_guard, DebugStatementType type, std::string msg);
void debug_print(const GM2D3Exception &gex);

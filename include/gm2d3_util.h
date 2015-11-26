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

enum class DebugStatementType {
    SUCCESS,
    WARNING,
    ERROR
};

void debug_print(int level_guard, DebugStatementType type, std::string msg);

std::string axis_to_string(Axis axis);


void const print_logo();

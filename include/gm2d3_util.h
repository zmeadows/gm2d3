#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <string>

#include "termcolor.hpp"

#include "gm2d3_const.h"
#include <Fl/fl_draw.H>

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;

void debug_print(int level_guard, std::string msg);

std::string axis_to_string(Axis axis);

Fl_Color LIGHT_RED();
Fl_Color DARK_RED();

Fl_Color LIGHT_GREEN();
Fl_Color DARK_GREEN();

Fl_Color LIGHT_BLUE();
Fl_Color DARK_BLUE();

Fl_Color YELLOW();

void const print_logo();

#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>
#include <string>

#include "gm2d3_const.h"

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;

void debug_print(int level_guard, std::string msg);

std::string axis_to_string(Axis axis);

#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <math.h>

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;

void debug_print(int level_guard, std::string msg);

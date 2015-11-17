#pragma once

#include <iostream>

#include <libconfig.h++>
using namespace libconfig;

extern int gDEBUG_LEVEL;

void debug_print(int level_guard, std::string msg);

bool verify_config_base(std::unique_ptr<Config> const& cfg);
bool verify_config_controller(const Setting &axis_settings);

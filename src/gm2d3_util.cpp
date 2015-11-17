#include "gm2d3_util.h"
#include "gm2d3_version.h"

#include <iostream>
#include <string>


int gDEBUG_LEVEL = 0;

void
debug_print(int level_guard, std::string msg)
{
    if (level_guard <= gDEBUG_LEVEL)
    {
        std::cout << "[GM2D3::v" << GM2D3_VERSION_MAJOR << "." << GM2D3_VERSION_MINOR << "] " << msg << std::endl;
    }
}

bool
verify_config_base(std::unique_ptr<Config> const& cfg)
{
    try
    {
        const Setting& root = cfg->getRoot();
        const Setting &controllers = root["controllers"];
        unsigned int num_controllers = 0;

        if (controllers.exists("azimuthal")) {
            num_controllers++;
            const Setting &azcfg = controllers["azimuthal"];
            if(verify_config_controller(azcfg)) {} else return false;
        }

        if (controllers.exists("vertical")) {
            num_controllers++;
            const Setting &vtcfg = controllers["vertical"];
            if(verify_config_controller(vtcfg)) {} else return false;
        }

        if (controllers.exists("radial")) {
            num_controllers++;
            const Setting &rdcfg = controllers["radial"];
            if(verify_config_controller(rdcfg)) {} else return false;
        }

        if (num_controllers < 1) { return false; }
    }

    catch(const SettingNotFoundException &nfex)
    {
        debug_print(0, "setting not found!");
        return false;
    }

    return true;
}

bool
verify_config_controller(const Setting &c)
{
    try
    {
        if (!c.exists("type")) { return false; }
        if (!c.exists("resolution")) { return false; }
        if (!c.exists("bounds")) { return false; }
        if (!c.exists("cypher")) { return false; }
    }

    catch(const SettingNotFoundException &nfex)
    {
        debug_print(0, "setting not found!");
        return false;
    }

    return true;
}

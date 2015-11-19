#pragma once

#include "gm2d3_stage_controller.h"
#include "gm2d3_const.h"

#include <memory>
#include <iostream>
#include <map>

#include <libconfig.h++>
using namespace libconfig;

class RaspberryPiController : public StageController
{
    public:
        RaspberryPiController(const Setting &cfg);

        ControllerType controller_type(void) const { return ControllerType::RaspberryPi; }

    private:
        const unsigned int motor_cw_gpio, motor_ccw_gpio;
        std::map<unsigned int, Encoder> encoder_gpios;

        void internal_change_motor_state(MotorState m);
        int internal_monitor(void);
        void shutdown(void);

        static void static_encoder_callback(int gpio, int level, uint32_t tick, void *user);
        void encoder_callback(int gpio, int level, uint32_t tick);
};

std::map<unsigned int,Encoder> config_get_encoder_gpios(const Setting &c);

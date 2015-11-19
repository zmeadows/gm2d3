#pragma once

#include "gm2d3_stage_controller.h"
#include "gm2d3_const.h"

#include <memory>
#include <iostream>

class RaspberryPiController : public StageController
{
    public:
        RaspberryPiController(const Setting &cfg);

        ControllerType controller_type(void) const { return ControllerType::RaspberryPi; }

    private:
        const unsigned int motor_cw_gpio, motor_ccw_gpio;

        void internal_change_motor_state(MotorState m);
        int internal_monitor(void);
        void shutdown(void);
};

#pragma once

#include "gm2d3_stage_controller.h"

#include <libconfig.h++>
using namespace libconfig;

class FakeController : public StageController {
    public:
        FakeController(const Setting &c);
    private:
        ControllerType controller_type(void) const { return ControllerType::Fake; }
        void setup(void);
        void internal_change_motor_state(MotorState m);
        int internal_monitor(void);
        void shutdown(void);
};

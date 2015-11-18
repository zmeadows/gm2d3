#pragma once

#include "gm2d3_stage_controller.h"

#include <libconfig.h++>
using namespace libconfig;

class FakeController : public StageController {
    public:
        FakeController(const Setting &c);
    private:
        void start1(MotorDirection dir) { };
        void stop1(void) { };
        ControllerType controller_type(void) const { return ControllerType::Fake; }
};

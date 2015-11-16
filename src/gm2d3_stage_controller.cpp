#include "gm2d3_stage_controller.h"

#include "gm2d3_util.h"

// StageController::StageController(std::unique_ptr<Config> const& cfg)
// {
//
// }

void
StageController::stop(void)
{
    if (motor_state.first == false) {
        debug_print(1, "Motor appears to already be stopped. Issuing stop command anyway.");
    } else {
        stop1();
        motor_state.first = false;
    }
}

void
StageController::start(MotorDirection dir)
{
    if (motor_state.first == true && motor_state.second == dir) {
        debug_print(1, "Motor appears to already be turning in the desired direction. No action performed.");
    } else if (motor_state.first == false || motor_state.second != dir) {
        start1(dir);
        debug_print(1, "Motor changed directions.");
        motor_state.first = true;
        motor_state.second = dir;
    }
}



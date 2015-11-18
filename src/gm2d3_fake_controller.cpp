#include "gm2d3_fake_controller.h"

FakeController::FakeController(const Setting &c) :
    StageController(c)
{
}

void
FakeController::internal_change_motor_state(MotorState m)
{
}

int
FakeController::internal_monitor(void)
{
    return 0;
}

void
FakeController::shutdown(void)
{
}

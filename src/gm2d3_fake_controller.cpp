#include "gm2d3_fake_controller.h"

FakeController::FakeController(const Setting &c) :
    StageController(c)
{
}

void
FakeController::internal_change_motor_state(MotorState m)
{
    switch(m)
    {
        case MotorState::CW:
            std::cout << "CW" << std::endl;
            break;
        case MotorState::CCW:
            std::cout << "CCW " << std::endl;
            break;
        case MotorState::OFF:
            std::cout << "OFF " << std::endl;
            break;
    }
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

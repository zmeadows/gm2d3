#include "gm2d3_rpi_controller.h"

#include <pigpio.h>

RaspberryPiController::RaspberryPiController(const Setting &cfg)
    : StageController(cfg),
    motor_cw_gpio(unsigned(cfg.lookup("motor_cw_gpio"))),
    motor_ccw_gpio(unsigned(cfg.lookup("motor_ccw_gpio")))
{
    gpioSetMode(motor_cw_gpio, PI_OUTPUT);
    gpioSetMode(motor_ccw_gpio, PI_OUTPUT);
}

void
RaspberryPiController::internal_change_motor_state(MotorState m)
{
    switch(m)
    {
        case MotorState::CW:
            gpioWrite(motor_ccw_gpio, 0);
            gpioWrite(motor_cw_gpio, 1);
            break;
        case MotorState::CCW:
            gpioWrite(motor_cw_gpio, 0);
            gpioWrite(motor_ccw_gpio, 1);
            break;
        case MotorState::OFF:
            gpioWrite(motor_cw_gpio, 0);
            gpioWrite(motor_ccw_gpio, 0);
            break;
    }
}

int
RaspberryPiController::internal_monitor(void)
{
    return 0;
}

void
RaspberryPiController::shutdown(void)
{
}

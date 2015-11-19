#include "gm2d3_rpi_controller.h"

#include <pigpio.h>

RaspberryPiController::RaspberryPiController(const Setting &cfg)
    : StageController(cfg),
    motor_cw_gpio(unsigned(cfg.lookup("motor_cw_gpio"))),
    motor_ccw_gpio(unsigned(cfg.lookup("motor_ccw_gpio"))),
    encoder_gpios(config_get_encoder_gpios(cfg))
{
    gpioSetMode(motor_cw_gpio, PI_OUTPUT);
    gpioSetMode(motor_ccw_gpio, PI_OUTPUT);

    for (auto& g: encoder_gpios) {
        gpioSetMode(g.first, PI_INPUT);
        gpioSetAlertFuncEx(g.first, static_encoder_callback, this);
    }

}

void
RaspberryPiController::static_encoder_callback(int gpio, int level, uint32_t tick, void *user)
{
    RaspberryPiController *self = (RaspberryPiController*) user;
    self->encoder_callback(gpio, level, tick);
}

void
RaspberryPiController::encoder_callback(int gpio, int level, uint32_t tick)
{
    bool b = (level == 0) ? false : true;
    // prev_encoder_state[encoder_gpios[gpio]] = !(b);
    Encoder e = encoder_gpios[unsigned(gpio)];
    encoder_state[e] = b;

    std::cout << encoder_state[Encoder::A] << encoder_state[Encoder::B]
        << encoder_state[Encoder::C] << encoder_state[Encoder::D] << std::endl;
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

std::map<unsigned int, Encoder>
config_get_encoder_gpios(const Setting &c)
{
    std::map<unsigned int, Encoder> gpios;

    gpios[unsigned(c.lookup("gpioA"))] = Encoder::A;
    gpios[unsigned(c.lookup("gpioB"))] = Encoder::B;
    gpios[unsigned(c.lookup("gpioC"))] = Encoder::C;
    gpios[unsigned(c.lookup("gpioD"))] = Encoder::D;

    return gpios;
}

#pragma once

#include "gm2d3_const.h"

#include <map>
#include <array>
#include <utility>
#include <iostream>

enum class Encoder {
    A, // evenly spaced high-res clock
    B, // in quadrature with A
    C, // low-res absolute position clock
    D  // absolute position bit-wise encoding
};

const std::map<Encoder,bool> EMPTY_ENCODER_STATE =
    { {Encoder::A, false}, {Encoder::B, false}, {Encoder::C, false}, {Encoder::D, false} };

enum class ControllerType {
    RaspberryPi,
    Galil,
    Fake
};


class StageController {
    public:
        StageController( Axis _axis, double min_bound, double max_bound,
                std::map<int,double> _abs_code_cypher);

        const Axis axis;
        const std::pair<double,double> bounds;
        const std::map<int,double> abs_code_cypher;

        virtual void stop() = 0;
        virtual void start(MotorDirection dir) = 0;
        virtual void controller_type() = 0;

        double get_current_position() { return current_position; }
        std::pair<bool, MotorDirection> get_motor_state() { return motor_state; }
        bool is_calibrated() { return calibrated; }

    protected:
        std::array<bool, ABS_CODE_BITS> abs_code_accumulator;
        std::map<Encoder, bool> encoder_state;
        std::pair<bool, MotorDirection> motor_state;
        double current_position;
        bool calibrated;

};

class RaspberryPiController : public StageController
{
    public:
        RaspberryPiController(Axis _axis, double _min_bound, double _max_bound,
                std::map<int,double> _abs_code_cypher, int _cw_gpio, int _ccw_gpio);

        void move_forward() {
            std::cout << cw_gpio;
        };
        void move_backward() {
            std::cout << ccw_gpio;
        };

    private:
        const int cw_gpio, ccw_gpio;
};

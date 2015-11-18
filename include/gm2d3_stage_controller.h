#pragma once

#include "gm2d3_const.h"

#include <map>
#include <vector>
#include <utility>
#include <iostream>

#include <libconfig.h++>
using namespace libconfig;

enum class Encoder {
    A, // evenly spaced high-res clock
    B, // in quadrature with A
    C, // low-res absolute position clock
    D  // absolute position bit-wise encoding
};

enum class ControllerType {
    RaspberryPi,
    Galil,
    Fake
};

const std::map<Encoder,bool> EMPTY_ENCODER_STATE = {
    {Encoder::A, false},
    {Encoder::B, false},
    {Encoder::C, false},
    {Encoder::D, false}
};

std::pair<double,double> config_get_bounds(const Setting &c);
std::map<int,double> config_get_cypher(const Setting &c);

class StageController {
    public:
        StageController(const Setting &c);

        const std::pair<double,double> bounds;
        const std::map<int,double> cypher;

        void stop(void);
        void start(MotorDirection dir);
        // virtual void setup(void) = 0;
        // virtual void disconnect(void) = 0;

        double get_current_position() { return current_position; }

        std::pair<bool, MotorDirection> get_motor_state() { return motor_state; }

        bool is_calibrated() { return calibrated; }

    protected:
        virtual void stop1(void) = 0;
        virtual void start1(MotorDirection dir) = 0;
        virtual ControllerType controller_type(void) const = 0;

        std::vector<bool> cypher_accumulator;
        std::map<Encoder, bool> encoder_state;
        std::pair<bool, MotorDirection> motor_state;

        double resolution;
        double current_position;
        bool calibrated;
        const unsigned int cypher_bits;
};

class ControllerException {
    public:
        ControllerException(std::string _msg) : msg(_msg) {}
        const std::string msg;
};


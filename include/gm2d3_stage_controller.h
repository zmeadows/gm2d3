#pragma once

#include "gm2d3_const.h"

#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <thread>

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

        void move(double new_position);
        void change_motor_state(MotorState next_motor_state);

        double get_current_position() const { return current_position; }
        MotorState get_current_motor_state(void) const { return current_motor_state; }
        bool is_calibrated() const { return calibrated; }

    protected:
        void monitor(void);

        virtual void internal_change_motor_state(MotorState m) = 0;
        virtual int internal_monitor(void) = 0;
        virtual void shutdown(void) = 0;

        virtual ControllerType controller_type(void) const = 0;

        MotorState current_motor_state;
        std::vector<bool> cypher_accumulator;
        std::map<Encoder, bool> encoder_state;

        const double resolution;
        double current_position;
        double goal_position;
        bool calibrated;
        const unsigned int cypher_bits;
};

class ControllerException {
    public:
        ControllerException(std::string _msg) : msg(_msg) {}
        const std::string msg;
};


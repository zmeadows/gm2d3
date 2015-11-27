#pragma once

#include <vector>
#include <map>

enum class Axis {
    AZIMUTHAL,
    VERTICAL,
    RADIAL
};

const std::vector<Axis> ALL_AXES = { Axis::AZIMUTHAL, Axis::VERTICAL, Axis::RADIAL };

enum class MotorState {
    CW,
    CCW,
    OFF
};

const std::vector<MotorState> ALL_MOTOR_STATES = { MotorState::CW, MotorState::CCW, MotorState::OFF };


enum class Encoder {

    A, // evenly spaced high-res clock
    B, // in quadrature with A
    C, // low-res absolute position clock
    D  // absolute position bit-wise encoding
};

const std::vector<Encoder> ALL_ENCODERS = { Encoder::A, Encoder::B, Encoder::C, Encoder::D };

const std::map<Encoder,bool> EMPTY_ENCODER_STATE = {
    {Encoder::A, false},
    {Encoder::B, false},
    {Encoder::C, false},
    {Encoder::D, false}
};


const double JITTER_TIME = 0.1;

// GUI
const int WINDOW_EDGE_GAP = 20;
const int ADJACENT_SPACING = 5;
const int BOX_EDGE_GAP = 7;


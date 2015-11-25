#include "gm2d3_stage_controller.h"
#include "gm2d3_util.h"

#include <Fl/Fl.H>

StageController::StageController(Axis _axis, gui_encoder_callback _gec, const void *_gm2d3,
        const Setting &c) :
    bounds(config_get_bounds(c.lookup("bounds"))),
    cypher(config_get_cypher(c.lookup("cypher"))),
    gm2d3(_gm2d3),
    gec(_gec),
    resolution(double(c.lookup("resolution"))),
    current_position(0.0),
    goal_position(0.0),
    calibrated(false),
    cypher_bits(unsigned(c.lookup("cypher_bits"))),
    axis(_axis),
    current_motor_state(MotorState::OFF),
    current_encoder_state(EMPTY_ENCODER_STATE),
    previous_encoder_state(EMPTY_ENCODER_STATE)
{
        if ((bounds.second - bounds.first) <= 0) {
            throw ControllerException("Invalid controller bounds");
        }

        if (resolution <= 0 || resolution >= bounds.second - bounds.first) {
            throw ControllerException("Invalid resolution");
        }

        for (auto& kv : cypher)
        {
            if (kv.second > bounds.second || kv.second < bounds.first)
            {
                throw ControllerException("Cypher value outside min/max bounds");
            }
        }

        cypher_accumulator.resize(cypher_bits);
        std::fill(cypher_accumulator.begin(), cypher_accumulator.end(), false);

        high_resolution_clock::time_point init_time = high_resolution_clock::now();
        for (auto &e : ALL_ENCODERS)
        {
            last_encoder_updates[e] = init_time;
        }
}

void
StageController::change_motor_state(MotorState next_motor_state)
{

    if (next_motor_state == MotorState::OFF)
    {
        if (current_motor_state == MotorState::OFF)
        {
            debug_print(1, "Motor appears to already be stopped. Issuing stop command again.");
        }

        internal_change_motor_state(MotorState::OFF);
        current_motor_state = MotorState::OFF;
    }

    else
    {
        if (current_motor_state == next_motor_state)
        {
            debug_print(1, "Motor appears to already be moving in that direction.");
        }

        else
        {
            if (current_motor_state != MotorState::OFF) internal_change_motor_state(MotorState::OFF);
            internal_change_motor_state(next_motor_state);
            current_motor_state = next_motor_state;
        }
    }
}

void
StageController::monitor()
{
//     int ret_code = internal_monitor();
//
//     switch (ret_code)
//     {
//         case 0:
//             debug_print(1, "successfully moved motor");
//         case -1:
//             debug_print(1, "failed to move motor");
//         default:
//             break;
//     }
}

void
StageController::update_encoder_state(Encoder e, bool state)
{

    high_resolution_clock::time_point current_time = high_resolution_clock::now();

    duration<double> time_span = duration_cast<duration<double>>
        (current_time - last_encoder_updates[e]);

    // TODO: add exceptions here
    if (time_span.count() < 0) { return; }

    if (time_span.count() < JITTER_TIME) { return; }

    if (get_current_motor_state() == MotorState::OFF) { return; }

    std::lock_guard<std::mutex> guard(encoder_mutex);

    // TODO: check that this state transition makes sense, throw exception if it doesn't
    previous_encoder_state[e] = !state;
    current_encoder_state[e] = state;

    double delta = resolution/2.0 * (get_current_motor_state() == MotorState::CW ?  1 : -1);

    switch (e)
    {
        case Encoder::A:
            current_position += delta;
            break;

        case Encoder::B:
            current_position += delta;
            break;

        case Encoder::C:
            break;

        case Encoder::D:
            break;
    }

    // std::cout << axis_to_string(axis) << " :"
    //     << current_encoder_state[Encoder::A]
    //     << current_encoder_state[Encoder::B]
    //     << current_encoder_state[Encoder::C]
    //     << current_encoder_state[Encoder::D]
    //     << " :" << time_span.count()
    //     << std::endl;
    // std::cout << std::endl;

    last_encoder_updates[e] = current_time;
    alert_gui(e, state ? 1 : 0);
}

void
StageController::move(double new_position)
{
    if (new_position > bounds.second || new_position < bounds.first) {
        debug_print(0, "Requested position not within stage bounds!");
        return;
    }

    // TODO: does this constraint need to be increased/relaxed?
    if (fabs(new_position - current_position) <= resolution) {
        debug_print(0, "Already at position to within stage resolution");
        return;
    }

    goal_position = new_position;

    std::thread monitor_thread;

    // TODO: disable manual control
    if (new_position > current_position) {
        change_motor_state(MotorState::CCW);

        Fl::lock();
        monitor_thread = std::thread([this]() {
            monitor();
        });
        monitor_thread.detach();
        Fl::unlock();
    }

    if (new_position < current_position) {
        change_motor_state(MotorState::CW);
        monitor();
    }
}

std::pair<double,double>
config_get_bounds(const Setting &c)
{
    double min, max;
    min = double(c.lookup("min"));
    max = double(c.lookup("max"));
    return std::make_pair(min, max);
}

std::map<int,double>
config_get_cypher(const Setting &c)
{
    std::map<int,double> cypher;
    int i;
    for (i = 0; i < c.getLength(); i++) {
        cypher[i] = double(c[i]);
    }
    return cypher;
}

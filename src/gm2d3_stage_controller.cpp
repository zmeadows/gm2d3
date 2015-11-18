#include "gm2d3_stage_controller.h"
#include "gm2d3_util.h"

#include <thread>
#include <Fl/Fl.H>

StageController::StageController(const Setting &c) :
    bounds(config_get_bounds(c.lookup("bounds"))),
    cypher(config_get_cypher(c.lookup("cypher"))),
    current_motor_state(MotorState::OFF),
    encoder_state(EMPTY_ENCODER_STATE),
    resolution(double(c.lookup("resolution"))),
    current_position(0.0),
    goal_position(0.0),
    calibrated(false),
    cypher_bits(unsigned(c.lookup("cypher_bits")))
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
    int ret_code = internal_monitor();

    switch (ret_code)
    {
        case 0:
            debug_print(1, "successfully moved motor");
        case -1:
            debug_print(1, "failed to move motor");
        default:
            break;
    }
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

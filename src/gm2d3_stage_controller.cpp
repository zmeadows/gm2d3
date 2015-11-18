#include "gm2d3_stage_controller.h"

#include "gm2d3_util.h"

StageController::StageController(const Setting &c) :
    bounds(config_get_bounds(c.lookup("bounds"))),
    cypher(config_get_cypher(c.lookup("cypher"))),
    encoder_state(EMPTY_ENCODER_STATE),
    motor_state(std::make_pair(false, MotorDirection::CW)),
    resolution(double(c.lookup("resolution"))),
    current_position(0.0),
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
StageController::stop(void)
{
    if (motor_state.first == false) {
        debug_print(1, "Motor appears to already be stopped. Issuing stop command anyway.");
        stop1();
    } else {
        stop1();
        motor_state.first = false;
    }
}

void
StageController::start(MotorDirection dir)
{
    if (motor_state.first == true && motor_state.second == dir) {
        debug_print(1, "Motor appears to already be turning in the desired direction. No action performed.");
    } else if (motor_state.first == false || motor_state.second != dir) {
        start1(dir);
        debug_print(1, "Motor changed directions.");
        motor_state.first = true;
        motor_state.second = dir;
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
    std::cout << c.getLength() <<std::endl;
    for (i = 0; i < c.getLength(); i++) {
        cypher[i] = double(c[i]);
    }
    return cypher;
}

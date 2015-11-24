#pragma once

#include "gm2d3_const.h"

#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std::chrono;

#include <libconfig.h++>
using namespace libconfig;

enum class ControllerType {
    RaspberryPi,
    Galil,
    Fake
};

std::pair<double,double> config_get_bounds(const Setting &c);
std::map<int,double> config_get_cypher(const Setting &c);

typedef void (*gui_encoder_callback)(Axis, Encoder, int, const void *);

class StageController {
    public:
        StageController(Axis _axis, gui_encoder_callback _gec, const void *_gm2d3,
                const Setting &c);

        const std::pair<double,double> bounds;
        const std::map<int,double> cypher;

        void move(double new_position);
        void change_motor_state(MotorState next_motor_state);

        double get_current_position() const { return current_position; }
        MotorState get_current_motor_state(void) const { return current_motor_state; }
        bool is_calibrated() const { return calibrated; }
        std::map<Encoder, bool> get_encoder_state(void) { 
            std::lock_guard<std::mutex> guard(encoder_mutex);
            return current_encoder_state; 
        }

        virtual ControllerType controller_type(void) const = 0;

    protected:
        void monitor(void);
        void update_encoder_state(Encoder e, bool state);

        virtual void internal_change_motor_state(MotorState m) = 0;
        virtual void shutdown(void) = 0;


    private:
        const void *gm2d3;
        const gui_encoder_callback gec;

        void alert_gui(Encoder e, int level) { gec(axis, e, level, gm2d3); }

        const double resolution;
        double current_position, goal_position;
        bool calibrated;
        const unsigned int cypher_bits;
        const Axis axis;

        MotorState current_motor_state;
        std::vector<bool> cypher_accumulator;

        std::mutex encoder_mutex;
        std::map<Encoder, bool> current_encoder_state;
        std::map<Encoder, bool> previous_encoder_state;
        std::map<Encoder, high_resolution_clock::time_point> last_encoder_updates;
};

class ControllerException {
    public:
        ControllerException(std::string _msg) : msg(_msg) {}
        const std::string msg;
};


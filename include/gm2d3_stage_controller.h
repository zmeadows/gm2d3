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

typedef void (*gui_encoder_callback)(Axis, Encoder, bool, const high_resolution_clock::time_point tp, const void *);
typedef void (*gui_shutdown_callback)(Axis, const void *);

std::pair<Encoder, bool> next_transition(MotorState m, bool A, bool B);

class StageController {
    public:
        StageController(
                Axis _axis,
                gui_encoder_callback _gec,
                gui_shutdown_callback _gsc,
                const void *_gm2d3,
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

        bool is_jittering() const { return jittering; }

        virtual ControllerType controller_type(void) const = 0;

        void shutdown(void);

    protected:
        void monitor(void);
        void update_encoder_state(Encoder e, bool state,
                high_resolution_clock::time_point tp);

        virtual void internal_change_motor_state(MotorState m) = 0;
        virtual void internal_shutdown(void) = 0;

    private:

        const high_resolution_clock::time_point get_last_transition_time(Encoder e);

        unsigned int jitters_rejected;
        bool jittering;

        const void *gm2d3;
        const gui_encoder_callback gec;
        const gui_shutdown_callback gsc;
        void alert_gui(Encoder e, bool state, high_resolution_clock::time_point tp) {
            gec(axis, e, state, tp, gm2d3);
        }

        const double resolution, middle_position;
        double current_position, goal_position;
        bool calibrated;
        const unsigned int cypher_bits;
        const Axis axis;

        MotorState current_motor_state;

        std::mutex encoder_mutex;
        std::map<Encoder, bool> current_encoder_state;
        std::map<Encoder, bool> previous_encoder_state;

        std::map<Encoder, std::vector< std::pair<high_resolution_clock::time_point,bool>>> encoder_history;
};


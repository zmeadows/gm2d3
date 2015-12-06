#pragma once

#include "gm2d3_const.h"

#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <thread>

#include <libconfig.h++>
using namespace libconfig;

enum class ControllerType
{
    RaspberryPi,
    Galil,
    Fake
};

typedef std::function<void(Encoder, bool)> gui_encoder_callback;

std::pair<double,double> config_get_bounds(const Setting &c);
std::map<int,double> config_get_cypher(const Setting &c);
std::pair<Encoder, bool> next_expected_transition(MotorState m, bool A, bool B);

class StageController
{
public:
    StageController(Axis axis, gui_encoder_callback _gec, const Setting &c);
    virtual ~StageController(void) {};

    void change_motor_state(MotorState next_motor_state);
    void move(double new_position);

    double get_current_position() const { return current_position_; }
    double get_resolution() const { return resolution_; }
    MotorState get_current_motor_state(void) const { return current_motor_state_; }
    std::pair<double,double> get_bounds(void) const { return bounds_; }
    bool is_lost() const { return decoder.is_lost(); }

    // TODO: move this to .h file
    std::map<Encoder, bool> get_encoder_state(void)
    {
        std::lock_guard<std::mutex> guard(encoder_mutex_);
        return current_encoder_state_;
    }

    virtual ControllerType controller_type(void) const = 0;

    void shutdown(void);

protected:
    void process_encoder_transition(Encoder e, bool state, time_point tp);
    virtual void internal_change_motor_state(MotorState m) = 0;
    virtual void internal_shutdown(void) = 0;

private:
    const Axis axis_;

    class CypherDecoder {
        public:
            CypherDecoder(const Setting &c);
            bool is_lost() const { return lost_; }

            time_point get_last_transition_time(Encoder e);
            unsigned int get_num_cypher_bits() const {return cypher_bits_; }
            std::map<int,double> get_cypher() const {return cypher_; }

            void add_transition_to_history(Encoder e, bool state, time_point tp);
        private:
            bool lost_;
            std::map<Encoder, std::vector<std::pair<time_point,bool>>> encoder_history_;
            const unsigned int cypher_bits_;
            const std::map<int,double> cypher_;
    } decoder;

    bool is_jittering_;
    unsigned int jitters_rejected_;
    const std::pair<double,double> bounds_;

    gui_encoder_callback gec_;

    const double resolution_, middle_position_;
    double current_position_, goal_position_;

    MotorState current_motor_state_;

    std::mutex encoder_mutex_;
    std::map<Encoder, bool> current_encoder_state_;
    std::map<Encoder, bool> previous_encoder_state_;

};


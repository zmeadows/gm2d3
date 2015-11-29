#include "gm2d3_stage_controller.h"
#include "gm2d3_util.h"

#include <chrono>
using namespace std::chrono;

StageController::StageController( Axis _axis, gui_encoder_callback _gec,
        gui_shutdown_callback _gsc, const void *_gm2d3, const Setting &c)
    : bounds(config_get_bounds(c.lookup("bounds"))),
    cypher(config_get_cypher(c.lookup("cypher"))),
    axis(_axis), jitters_rejected(0), jittering(false),
    gm2d3(_gm2d3), gec(_gec), gsc(_gsc), resolution(double(c.lookup("resolution"))),
    middle_position(bounds.first + (bounds.second - bounds.first)/2.0),
    current_position(middle_position), goal_position(middle_position),
    calibrated(false), cypher_bits(unsigned(c.lookup("cypher_bits"))),
    current_motor_state(MotorState::OFF), current_encoder_state(EMPTY_ENCODER_STATE),
    previous_encoder_state(EMPTY_ENCODER_STATE)
{
    if ((bounds.second - bounds.first) <= 0) {
        throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Invalid controller bounds");
    }

    if (resolution <= 0 || resolution >= bounds.second - bounds.first) {
        throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Invalid resolution");
    }

    for (auto& kv : cypher)
    {
        if (kv.second > bounds.second || kv.second < bounds.first)
        {
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Cypher value outside min/max bounds");
        }
    }

    high_resolution_clock::time_point init_time = high_resolution_clock::now();
    for (auto &e : ALL_ENCODERS)
    {
        for (auto &s : { true, false }) {
            encoder_history[e].push_back(std::make_pair(init_time, s));
        }
    }
}

void
StageController::change_motor_state(MotorState next_motor_state)
{

    if (next_motor_state == MotorState::OFF)
    {
        if (current_motor_state == MotorState::OFF)
        {
            debug_print(1, DebugStatementType::WARNING, "Motor appears to already be stopped. Issuing stop command again.");
        }

        debug_print(1, DebugStatementType::GENERIC, "Stopping motor...");
        internal_change_motor_state(MotorState::OFF);
        current_motor_state = MotorState::OFF;
    }

    else
    {
        if (current_motor_state == next_motor_state)
        {
            debug_print(1, DebugStatementType::WARNING, "Motor appears to already be moving in that direction.");
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
}

void
StageController::update_encoder_state( Encoder e, bool state,
        high_resolution_clock::time_point tp)
{

    // LOCK ENCODER STATE
    // insures that all encoder transitions are processed in order,
    // as long as update_encoder_state is called in order
    encoder_mutex.lock();

    duration<double> time_span = duration_cast<duration<double>>
        (tp - get_last_transition_time(e));


    try
    {
        // OUT-OF-TIME TRANSITION PROCESSING
        if (time_span.count() < 0) {
            throw make_gm2d3_exception(GM2D3Exception::Type::Programmer,
                    "Attempted to process encoder events out of order. This should never happen!");
        }

        if (time_span.count() < JITTER_TIME)
        {
            if (!jittering) jittering = true;
            jitters_rejected++;
            return;
        }
        else
        {
            if (jittering) // DONE JITTERING
            {
                jittering = false;
                std::string jitter_msg = "Jittering detected on ";
                jitter_msg += axis_to_string(axis);
                jitter_msg += " axis: (" ;
                jitter_msg += std::to_string(jitters_rejected);
                jitter_msg += " rejected jitter transitions so far)";
                throw make_gm2d3_exception(GM2D3Exception::Type::SafetyWarning, jitter_msg );
            }
        }

        if (get_current_motor_state() == MotorState::OFF) {
            throw make_gm2d3_exception(GM2D3Exception::Type::SafetyWarning,
                    "Encoder logic transition occurred while motor was off! Ignoring...");
        }
    }

    catch (GM2D3Exception &gex)
    {
        debug_print(gex);

        if (gex.type != GM2D3Exception::Type::SafetyWarning) shutdown();
        return;
    }

    catch (...)
    {
        debug_print(0, DebugStatementType::ERROR,
                "Unrecognized error occurred, shutting down...");
        shutdown();
    }

    // add transition to the history
    if (encoder_history[e].size() > 10) encoder_history[e].clear();
    encoder_history[e].push_back(std::make_pair(tp,state));

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

    encoder_mutex.unlock();
    alert_gui(e, state, tp);
}

const high_resolution_clock::time_point
StageController::get_last_transition_time(Encoder e)
{
    return encoder_history[e].back().first;
}

void
StageController::move(double new_position)
{
    if (new_position > bounds.second || new_position < bounds.first) {
        debug_print(0, DebugStatementType::WARNING, "Requested position not within stage bounds!");
        return;
    }

    // TODO: does this constraint need to be increased/relaxed?
    if (fabs(new_position - current_position) <= resolution) {
        debug_print(0, DebugStatementType::WARNING, "Already at position to within stage resolution");
        return;
    }

    goal_position = new_position;

    std::thread monitor_thread;

    // TODO: disable manual control
    if (new_position > current_position) {
        change_motor_state(MotorState::CCW);

        monitor_thread = std::thread([this]() {
                monitor();
                });
        monitor_thread.detach();
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

std::pair<Encoder, bool>
next_transition(MotorState m, bool A, bool B)
{
    switch (m)
    {
        case MotorState::CW:
            if (A && !B) {
                return std::make_pair(Encoder::B, true);
            } else if (A && B) {
                return std::make_pair(Encoder::A, false);
            } else if (!A && B) {
                return std::make_pair(Encoder::B, false);
            } else {
                return std::make_pair(Encoder::A, true);
            }
            break;

        case MotorState::CCW:
            if (A && !B) {
                return std::make_pair(Encoder::A, false);
            } else if (A && B) {
                return std::make_pair(Encoder::B, false);
            } else if (!A && B) {
                return std::make_pair(Encoder::A, true);
            } else {
                return std::make_pair(Encoder::B, true);
            }
            break;

        case MotorState::OFF:
            throw make_gm2d3_exception(GM2D3Exception::Type::Programmer,
                    "Fake controller attempted to change encoder state while \
                    motor was off!");
            break;
    }
}

void
StageController::shutdown(void)
{
    debug_print(2, DebugStatementType::ATTEMPT, "Shutting down " + axis_to_string(axis) + " stage...");

    change_motor_state(MotorState::OFF);
    internal_shutdown();

    gsc(axis, gm2d3);
}

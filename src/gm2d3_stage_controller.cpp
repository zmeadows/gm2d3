#include "gm2d3_stage_controller.h"
#include "gm2d3_util.h"

#include <sstream>
#include <chrono>
using namespace std::chrono;

StageController::StageController(Axis axis, gui_encoder_callback _gec, const Setting &c)
    : axis_(axis),
    decoder(c),
    is_jittering_(false),
    jitters_rejected_(0),
    bounds_(config_get_bounds(c.lookup("bounds"))),
    gec_(_gec),
    resolution_(double(c.lookup("resolution"))),
    middle_position_(bounds_.first + (bounds_.second - bounds_.first)/2.0),
    current_position_(middle_position_),
    goal_position_(middle_position_),
    current_motor_state_(MotorState::OFF),
    current_encoder_state_(EMPTY_ENCODER_STATE),
    previous_encoder_state_(EMPTY_ENCODER_STATE)
{
    if ((bounds_.second - bounds_.first) <= decoder.get_num_cypher_bits() * resolution_)
    {
        throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Invalid controller bounds");
    }

    if (resolution_ <= 0 || resolution_ >= bounds_.second - bounds_.first)
    {
        throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Invalid resolution");
    }

    for (auto& kv : decoder.get_cypher())
    {
        if (kv.second > bounds_.second || kv.second < bounds_.first)
        {
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, "Cypher value outside min/max bounds");
        }
    }

}

void
StageController::change_motor_state(MotorState next_motor_state)
{

    if (next_motor_state == MotorState::OFF)
    {
        if (current_motor_state_ == MotorState::OFF)
        {
            debug_print(1, DebugStatementType::WARNING, "Motor appears to already be stopped. Issuing stop command again.");
        }

        debug_print(1, DebugStatementType::GENERIC, "Stopping motor...");
        internal_change_motor_state(MotorState::OFF);
        current_motor_state_ = MotorState::OFF;
    }

    else
    {
        if (current_motor_state_ == next_motor_state)
        {
            debug_print(1, DebugStatementType::WARNING, "Motor appears to already be moving in that direction.");
        }

        else
        {
            if (current_motor_state_ != MotorState::OFF) internal_change_motor_state(MotorState::OFF);
            internal_change_motor_state(next_motor_state);
            current_motor_state_ = next_motor_state;
        }
    }
}

void
StageController::process_encoder_transition(Encoder e, bool state, high_resolution_clock::time_point tp)
{
    // LOCK ENCODER STATE
    // insures that all encoder transitions are processed in order,
    // as long as update_encoder_state is called in order
    encoder_mutex_.lock();

    duration<double> time_span = duration_cast<duration<double>>
        (tp - decoder.get_last_transition_time(e));


    try
    {
        // OUT-OF-TIME TRANSITION PROCESSING
        if (time_span.count() < 0)
        {
            throw make_gm2d3_exception(GM2D3Exception::Type::Programmer,
                                       "Attempted to process encoder events out of order. This should never happen!");
        }

        if (time_span.count() < JITTER_TIME)
        {
            if (!is_jittering_) is_jittering_ = true;
            jitters_rejected_++;
            return;
        }
        else
        {
            if (is_jittering_) // DONE is_jittering_
            {
                is_jittering_ = false;
                std::string jitter_msg = "is_jittering_ detected on ";
                jitter_msg += axis_to_string(axis_);
                jitter_msg += " axis: (" ;
                jitter_msg += std::to_string(jitters_rejected_);
                jitter_msg += " rejected jitter transitions so far)";
                throw make_gm2d3_exception(GM2D3Exception::Type::SafetyWarning, jitter_msg );
            }
        }

        if (get_current_motor_state() == MotorState::OFF)
        {
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
    decoder.add_transition_to_history(e,state,tp);

    previous_encoder_state_[e] = !state;
    current_encoder_state_[e] = state;

    double delta = resolution_/2.0 * (get_current_motor_state() == MotorState::CW ?  1 : -1);

    switch (e)
    {
    case Encoder::A:
        current_position_ += delta;
        break;

    case Encoder::B:
        current_position_ += delta;
        break;

    case Encoder::C:
        break;

    case Encoder::D:
        break;
    }

    encoder_mutex_.unlock();
    gec_(e, state);
}

high_resolution_clock::time_point
StageController::CypherDecoder::get_last_transition_time(Encoder e)
{
    return encoder_history_[e].back().first;
}

void
StageController::move(double new_position)
{
    if (new_position > bounds_.second || new_position < bounds_.first)
    {
        debug_print(0, DebugStatementType::WARNING, "Requested position not within stage bounds!");
        return;
    }

    // TODO: does this constraint need to be increased/relaxed?
    if (fabs(new_position - current_position_) <= resolution_)
    {
        debug_print(0, DebugStatementType::WARNING, "Already at position to within stage resolution");
        return;
    }

    goal_position_ = new_position;

    // TODO: disable manual control
    if (new_position > current_position_)
    {
        change_motor_state(MotorState::CCW);
    }

    if (new_position < current_position_)
    {
        change_motor_state(MotorState::CW);
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
    for (i = 0; i < c.getLength(); i++)
    {
        cypher[i] = double(c[i]);
    }
    return cypher;
}

std::pair<Encoder, bool>
next_expected_transition(MotorState m, bool A, bool B)
{
    switch (m)
    {
    case MotorState::CW:
        if (A && !B)
        {
            return std::make_pair(Encoder::B, true);
        }
        else if (A && B)
        {
            return std::make_pair(Encoder::A, false);
        }
        else if (!A && B)
        {
            return std::make_pair(Encoder::B, false);
        }
        else
        {
            return std::make_pair(Encoder::A, true);
        }
        break;

    case MotorState::CCW:
        if (A && !B)
        {
            return std::make_pair(Encoder::A, false);
        }
        else if (A && B)
        {
            return std::make_pair(Encoder::B, false);
        }
        else if (!A && B)
        {
            return std::make_pair(Encoder::A, true);
        }
        else
        {
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
    std::ostringstream shutdown_str;
    shutdown_str << "Shutting down " << axis_to_string(axis_) << " stage...";

    debug_print(2, DebugStatementType::ATTEMPT, shutdown_str.str());

    change_motor_state(MotorState::OFF);
    internal_shutdown();

    shutdown_str.str("");
    shutdown_str.clear();
    shutdown_str << "Successfully shut down " << axis_to_string(axis_) << " stage.";

    debug_print(2, DebugStatementType::SUCCESS, shutdown_str.str());
}

StageController::CypherDecoder::CypherDecoder(const Setting &c)
    : lost_(true),
    cypher_bits_(unsigned(c.lookup("cypher_bits"))),
    cypher_(config_get_cypher(c.lookup("cypher")))
{
    auto init_time = high_resolution_clock::now();
    for (auto &e : ALL_ENCODERS)
    {
        for (auto &s : { true, false })
        {
            encoder_history_[e].push_back(std::make_pair(init_time, s));
        }
    }
}

void
StageController::CypherDecoder::add_transition_to_history(Encoder e, bool state, high_resolution_clock::time_point tp)
{
    encoder_history_[e].push_back(std::make_pair(tp, state));
}

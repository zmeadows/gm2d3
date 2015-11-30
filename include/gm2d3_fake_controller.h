#pragma once

#include "gm2d3_stage_controller.h"
#include "gm2d3_util.h"

#include <chrono>
using namespace std::chrono;

#include <libconfig.h++>
using namespace libconfig;

class FakeController : public StageController
{
public:
    FakeController(Axis _axis, gui_encoder_callback _gec, gui_shutdown_callback _gsc,
                   const void *_gm2d3, const Setting &c);
private:
    ControllerType controller_type(void) const
    {
        return ControllerType::Fake;
    }
    void internal_change_motor_state(MotorState m);
    void internal_shutdown(void);

    void maybe_jitter(Encoder e, bool init_state);

    void detach_motor_mover_thread(MotorState m);

    std::thread motor_mover;

    const int timestep_us;
    bool keep_moving;

    static constexpr double JITTER_PROBABILITY = 0.1;
};

#include "gm2d3_fake_controller.h"
#include <unistd.h>

FakeController::FakeController(Axis axis, gui_encoder_callback gec, const Setting &c)
    : StageController(axis, gec, c), timestep_us(1e5), keep_moving(false)
{
}

void
FakeController::internal_change_motor_state(MotorState m)
{
    keep_moving = false;
    if (motor_mover.joinable()) motor_mover.join();

    if (m != MotorState::OFF && m != get_current_motor_state())
    {
        keep_moving = true;
        motor_mover = std::thread(&FakeController::detach_motor_mover_thread, this, m);
    }
}

void
FakeController::detach_motor_mover_thread(MotorState m)
{
    bool A, B;
    std::map<Encoder, bool> em;

    std::pair<Encoder, bool> nt;
    std::this_thread::sleep_for(std::chrono::microseconds(timestep_us));
    while(keep_moving)
    {
        em = get_encoder_state();
        A = em[Encoder::A];
        B = em[Encoder::B];

        nt = next_expected_transition(m, A, B);

        process_encoder_transition(nt.first, nt.second, high_resolution_clock::now());
        // maybe_jitter(nt.first, nt.second);

        std::this_thread::sleep_for(std::chrono::microseconds(timestep_us));
    }
}

void
FakeController::internal_shutdown(void)
{
}

// TODO: fix. this is broken.
void
FakeController::maybe_jitter(Encoder e, bool init_state)
{
    if (((double) rand() / (RAND_MAX)) > JITTER_PROBABILITY)
    {
        return;
    }

    const unsigned int num_jitters = (unsigned) 10 * ((double) rand() / (RAND_MAX));

    debug_print(2, DebugStatementType::GENERIC, "Generating " + std::to_string(num_jitters)
            + " fake jitters...");

    bool next_state = !init_state;
    for (int n = 0; n < num_jitters; n++)
    {
        debug_print(2, DebugStatementType::GENERIC, "jittered.");
        unsigned int ms_delay = (int) ( 20.0 * ((double) rand() / (RAND_MAX)));
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_delay));
        process_encoder_transition(e, next_state, high_resolution_clock::now());
        next_state = !next_state;
    }

    process_encoder_transition(e, init_state, high_resolution_clock::now());
}


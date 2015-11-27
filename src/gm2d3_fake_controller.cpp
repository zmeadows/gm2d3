#include "gm2d3_fake_controller.h"


FakeController::FakeController(Axis _axis, gui_encoder_callback _gec, 
        gui_shutdown_callback _gsc, const void *_gm2d3, const Setting &c)
    : StageController(_axis, _gec, _gsc, _gm2d3, c), timestep_us(3e5), keep_moving(false)
{
}

void
FakeController::internal_change_motor_state(MotorState m)
{

    if (m == MotorState::OFF) {
        keep_moving = false;
        motor_mover.join();
        return;
    }

    if (keep_moving == true && m != get_current_motor_state()) {
        keep_moving = false;
        motor_mover.join();
    }

    auto fake_mover = [this, m]() -> void
    {
        bool A, B;
        std::map<Encoder, bool> em;

        std::this_thread::sleep_for(std::chrono::microseconds(this->timestep_us));

        while(this->keep_moving)
        {
            em = this->get_encoder_state();
            A = em[Encoder::A];
            B = em[Encoder::B];

            std::pair<Encoder, bool> nt = next_transition(m, A, B);

            update_encoder_state(nt.first, nt.second, high_resolution_clock::now());
            maybe_jitter(nt.first, nt.second);

            std::this_thread::sleep_for(std::chrono::microseconds(this->timestep_us));
        }
    };

    keep_moving = true;
    motor_mover = std::thread(fake_mover);
}

void
FakeController::internal_shutdown(void)
{
}

void
FakeController::maybe_jitter(Encoder e, bool init_state)
{
    if (((double) rand() / (RAND_MAX)) > JITTER_PROBABILITY) { return; }

    bool next_state = !init_state;
    for (int n = 0; n < (int) 20 * ((double) rand() / (RAND_MAX)); n++)
    {
        unsigned int ms_delay = (int) ( 10.0 * ((double) rand() / (RAND_MAX)));
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_delay));
        update_encoder_state(e, next_state, high_resolution_clock::now());
        next_state = !next_state;
    }

    update_encoder_state(e, init_state, high_resolution_clock::now());
}


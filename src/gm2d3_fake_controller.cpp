#include "gm2d3_fake_controller.h"


FakeController::FakeController(Axis _axis, gui_encoder_callback _gec, const void *_gm2d3,
        const Setting &c) : StageController(_axis, _gec, _gm2d3, c),
    timestep_us(3e5),
    keep_moving(false)
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

        if (this->keep_moving)
            std::this_thread::sleep_for(std::chrono::microseconds(this->timestep_us));

        while(this->keep_moving)
        {
            em = this->get_encoder_state();
            A = em[Encoder::A];
            B = em[Encoder::B];

            switch (m)
            {
                case MotorState::CW:
                    if (A && !B) {
                        update_encoder_state(Encoder::B, true, high_resolution_clock::now());
                        maybe_jitter(Encoder::B, true);
                    } else if (A && B) {
                        update_encoder_state(Encoder::A, false, high_resolution_clock::now());
                        maybe_jitter(Encoder::A, false);
                    } else if (!A && B) {
                        update_encoder_state(Encoder::B, false, high_resolution_clock::now());
                        maybe_jitter(Encoder::B, false);
                    } else {
                        update_encoder_state(Encoder::A, true, high_resolution_clock::now());
                        maybe_jitter(Encoder::A, true);
                    }
                    break;

                case MotorState::CCW:
                    if (A && !B) {
                        update_encoder_state(Encoder::A, false, high_resolution_clock::now());
                        maybe_jitter(Encoder::A, false);
                    } else if (A && B) {
                        update_encoder_state(Encoder::B, false, high_resolution_clock::now());
                        maybe_jitter(Encoder::B, false);
                    } else if (!A && B) {
                        update_encoder_state(Encoder::A, true, high_resolution_clock::now());
                        maybe_jitter(Encoder::A, true);
                    } else {
                        update_encoder_state(Encoder::B, true, high_resolution_clock::now());
                        maybe_jitter(Encoder::B, true);
                    }
                    break;

                case MotorState::OFF:
                    break;
            }

            std::this_thread::sleep_for(std::chrono::microseconds(this->timestep_us));
        }
    };

    keep_moving = true;
    motor_mover = std::thread(fake_mover);
}

void
FakeController::shutdown(void)
{
    keep_moving = false;
    motor_mover.join();
}

void
FakeController::maybe_jitter(Encoder e, bool init_state)
{
    if (((double) rand() / (RAND_MAX)) > JITTER_PROBABILITY) { return; }

    bool next_state = !init_state;
    for (int n = 0; n < 10; n++)
    {
        unsigned int ms_delay = (int) ( 20.0 * ((double) rand() / (RAND_MAX)));
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_delay));
        update_encoder_state(e, next_state, high_resolution_clock::now());
        next_state = !next_state;
    }

    update_encoder_state(e, init_state, high_resolution_clock::now());
}


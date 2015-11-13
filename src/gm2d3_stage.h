#ifndef GM2D3_STAGE_H
#define GM2D3_STAGE_H

#include <map>

enum class Axis {
    AZIMUTHAL,
    VERTICAL,
    RADIAL
};

enum class Encoder {
    A, // evenly spaced clock
    B, // in quadrature with A
    C, // absolute position clock
    D  // absolute position bit-wise encoding
};

const std::map<Encoder,bool> EMPTY_ENCODER_STATE =
    { {Encoder::A, false}, {Encoder::B, false}, {Encoder::C, false}, {Encoder::D, false} };

class StageController {
    public:
        StageController( Axis _axis, double _min_bound, double _max_bound,
                std::map<int,double> _abs_enc, int _abs_code_bits);

        const Axis axis;
        const double min_bound, max_bound;
        const std::map<int,double> abs_code_to_position;

        virtual void stop() = 0;
        virtual void move_forward() = 0;
        virtual void move_backward() = 0;

        double get_current_position() { return cur_pos; }

    private:
        const int abs_code_bits;
        double cur_pos;
        bool calibrated;
        std::map<Encoder, bool> encoder_state;
        bool *abs_code_accum;

};

#endif

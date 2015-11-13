#include "gm2d3_stage.h"

StageController::StageController(Axis _axis, double _min_bound, double _max_bound,
                std::map<int,double> _abs_enc, int _abs_code_bits) :
    axis(_axis),
    min_bound(_min_bound),
    max_bound(_max_bound),
    abs_code_bits(_abs_code_bits),
    encoder_state(EMPTY_ENCODER_STATE),
    cur_pos(0.0),
    calibrated(false)
{
    abs_code_accum = new bool[abs_code_bits];
    std::fill_n(abs_code_accum, abs_code_bits, false);
}

RaspberryPiController::RaspberryPiController(Axis _axis, double _min_bound, double _max_bound,
        std::map<int,double> _abs_enc, int _abs_code_bits, int _cw_gpio, int _ccw_gpio) :
    StageController(_axis, _min_bound, _max_bound, _abs_enc, _abs_code_bits),
    cw_gpio(_cw_gpio),
    ccw_gpio(_ccw_gpio)
{
}

#include "gm2d3_stage.h"

StageController::StageController(Axis _axis, double _min_bound, double _max_bound,
                std::map<int,double> _abs_code_cypher) :
    axis(_axis),
    bounds(std::make_pair(_min_bound, _max_bound)),
    abs_code_cypher(_abs_code_cypher),
    encoder_state(EMPTY_ENCODER_STATE),
    current_position(0.0),
    calibrated(false)
{
}

RaspberryPiController::RaspberryPiController(Axis _axis, double _min_bound, double _max_bound,
        std::map<int,double> _abs_code_cypher, int _cw_gpio, int _ccw_gpio) :
    StageController(_axis, _min_bound, _max_bound, _abs_code_cypher),
    cw_gpio(_cw_gpio),
    ccw_gpio(_ccw_gpio)
{
}

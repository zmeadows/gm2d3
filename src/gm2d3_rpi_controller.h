#pragma once

#include "gm2d3_stage_controller.h"
#include "gm2d3_const.h"

class RaspberryPiController : public StageController
{
    public:
        RaspberryPiController(std::unique_ptr<Config> const& cfg);

        void move_forward() {
            std::cout << cw_gpio;
        };
        void move_backward() {
            std::cout << ccw_gpio;
        };

    private:
        const int cw_gpio, ccw_gpio;
};

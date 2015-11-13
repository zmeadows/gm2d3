#pragma once

#include <map>

#include "gm2d3_window.h"
#include "gm2d3_stage.h"

class GM2D3 {
    public:
        GM2D3(int window_width, int window_height);
        void attach_controller(StageController *controller);

    private:
        GM2D3Window *window;
        std::map<Axis, StageController*> controllers;
};

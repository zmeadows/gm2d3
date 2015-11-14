#pragma once

#include <map>

#include "gm2d3_window.h"
#include "gm2d3_stage.h"

class GM2D3 {
    public:
        GM2D3(int window_width, int window_height);
        virtual ~GM2D3() {};
        void attach_controller(std::unique_ptr<StageController> c);

    private:
        std::unique_ptr<GM2D3Window> window;
        std::map<Axis, std::unique_ptr<StageController>> controllers;
};

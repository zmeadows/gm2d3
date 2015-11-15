#pragma once

#include "gm2d3_window.h"
#include "gm2d3_stage.h"

#include <map>

#include <libconfig.h++>
using namespace libconfig;

class GM2D3 {
    public:
        GM2D3(int window_width, int window_height);
        virtual ~GM2D3() {};

        void attach_controller(std::unique_ptr<RaspberryPiController> c);
        void detach_controllers(void);

        static void static_load_config_callback(Fl_Widget *_config_loader, void *gm2d3);
        void load_config_callback(Fl_Widget *_config_loader);

    private:
        std::unique_ptr<GM2D3Window> window;
        std::map<Axis, std::unique_ptr<StageController>> controllers;
        std::unique_ptr<Config> cfg;
};

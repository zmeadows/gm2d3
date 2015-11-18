#pragma once

#include "gm2d3_window.h"
#include "gm2d3_stage_controller.h"

#include <map>
#include <memory>

#include <libconfig.h++>
using namespace libconfig;

class GM2D3 {
    public:
        GM2D3(int window_width, int window_height);
        virtual ~GM2D3() {};

    private:
        std::unique_ptr<GM2D3Window> window;
        std::map<Axis, std::unique_ptr<StageController>> controllers;
        std::unique_ptr<Config> cfg;

        bool process_config_file(void);
        void unprocess_config_file(void);
        void setup_callbacks(void);

        void attach_controller(Axis axis, ControllerType ct, const Setting &c);

        static void static_load_config_callback(Fl_Widget *, void *gm2d3);
        void load_config_callback(Fl_Widget *);
};

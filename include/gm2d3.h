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
        std::map<Axis, std::shared_ptr<StageController>> controllers;
        std::unique_ptr<Config> cfg;

        void detach_plot_threads(void);
        std::shared_ptr<bool> keep_updating_plots;
        bool keep_updating_indicators;
        bool keep_updating_stats;

        bool process_config_file(void);
        void unprocess_config_file(void);

        void attach_controller(Axis axis, ControllerType ct, const Setting &c);
        void setup_controllers(void);

        static void static_encoder_state_callback(Axis a, Encoder e, int level, const void *gm2d3);
        void encoder_state_callback(Axis a, Encoder e, int level);

        static void static_load_config_callback(Fl_Widget *, void *gm2d3);
        void load_config_callback(Fl_Widget *);

        static void static_manual_button_callback(Fl_Widget *button, void *gm2d3);
        void manual_button_callback(Fl_Widget *button);

        static void static_enable_plot_callback(Fl_Widget *enable_plot_checkbox, void *gm2d3);
        void enable_plot_callback(Fl_Widget *enable_plot_checkbox);
};

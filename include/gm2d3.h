#pragma once

#include "gm2d3_window.h"
#include "gm2d3_stage_controller.h"
#include "gm2d3_const.h"

#include <map>
#include <memory>

#include <libconfig.h++>
using namespace libconfig;

class GM2D3 {
    public:
        GM2D3(int window_width, int window_height);

        std::unique_ptr<GM2D3Window> window;

    private:
        enum class OperatingState {
            DETACHED,
            WAITING_ON_USER,
            CALIBRATION,
            AUTO_TRANSLATION,
            MANUAL_TRANSLATION,
            RESETTING,
            SHUTTING_DOWN
        };

        OperatingState gm2d3_state;

        std::map<Axis, std::shared_ptr<StageController>> controllers;
        std::unique_ptr<Config> cfg;

        void start_plot_threads(void);
        void cleanup_plot_threads(void);
        std::shared_ptr<bool> keep_updating_plots;
        std::vector<std::thread> plot_threads;

        void enable_indicators();
        void disable_indicators();
        bool keep_updating_indicators;
        // bool keep_updating_stats;

        void update_stats(void);

        void process_config_file(void);
        void reset(void);

        void create_controller(Axis axis, ControllerType ct, const Setting &c);
        void attach_controllers(void);
        void detach_controllers(void);

        static void static_encoder_state_callback(Axis a, Encoder e, bool state,
                high_resolution_clock::time_point tp, const void *gm2d3);
        void encoder_state_callback(Axis a, Encoder e, bool state, high_resolution_clock::time_point tp);

        static void static_shutdown_callback(Axis a, const void *gm2d3);
        void shutdown_callback(Axis a);

        static void static_load_config_callback(Fl_Widget *, void *gm2d3);
        void load_config_callback(Fl_Widget *);

        static void static_manual_button_callback(Fl_Widget *button, void *gm2d3);
        void manual_button_callback(Fl_Widget *button);

        static void static_kill_button_callback(Fl_Widget *button, void *gm2d3);
        void kill_button_callback();

        static void static_enable_plot_callback(Fl_Widget *enable_plot_checkbox, void *gm2d3);
        void enable_plot_callback(Fl_Widget *enable_plot_checkbox);

        static void static_enable_indicators_callback(Fl_Widget *enable_plot_checkbox, void *gm2d3);
        void enable_indicators_callback(Fl_Widget *enable_indicators_checkbox);

        static void static_exit_window_callback(Fl_Widget *gm2d3_window, void *gm2d3);
        void exit_window_callback(Fl_Widget *gm2d3_window);
};

void exit_gm2d3(void *gm2d3);

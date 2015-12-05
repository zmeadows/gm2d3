#pragma once

#include "gm2d3_window.h"
#include "gm2d3_stage_controller.h"
#include "gm2d3_const.h"

#include <map>
#include <memory>

#include <libconfig.h++>
using namespace libconfig;

class GM2D3
{
public:
    GM2D3(int window_width, int window_height);

    std::unique_ptr<GM2D3Window> window;

private:
    enum class OperatingState
    {
        DETACHED, // default blank slate (i.e. state of program when first opened)
        UNCALIBRATED, // controllers attached but uncalibrated
        CALIBRATING, // in the middle of calibration routine
        WAITING, // finished with calibration and waiting for user
        AUTO_TRANSLATION, // in the middle of auto translation routine
        MANUAL_TRANSLATION, // undergoing user-fed manual translation
        RESETTING // in the process of resetting
    } gm2d3_state;

    std::map<Axis, std::unique_ptr<StageController>> controllers;
    std::unique_ptr<Config> cfg;

    void disable_plots(void);
    void enable_plots(void);
    void update_plot(Axis a);
    bool keep_updating_plots;

    void enable_indicators();
    void disable_indicators();
    void update_indicator(Axis a, Encoder e, bool s);
    bool keep_updating_indicators;

    void enable_info(void);
    void disable_info(void);
    void update_info(Axis a);
    bool keep_updating_info;

    void process_config_file(void);
    void reset(void);

    void create_controller(Axis axis, ControllerType ct, const Setting &c);
    void attach_controllers(void);
    void detach_controllers(void);

    void encoder_transition_callback(Axis a, Encoder e, bool state);

    void enable_plot_checkbox_callback(Fl_Widget *enable_plot_checkbox);
    void enable_indicators_checkbox_callback(Fl_Widget *enable_indicators_checkbox);
    void enable_info_checkbox_callback(Fl_Widget *enable_info_checkbox);

    void manual_button_callback(Fl_Widget *button);
    void load_config_callback();
    void kill_button_callback();
    void exit_window_callback(Fl_Widget *gm2d3_window);
};

void exit_gm2d3(void *gm2d3);

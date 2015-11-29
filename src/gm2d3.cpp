#include "gm2d3.h"

#include "gm2d3_util.h"
#include "gm2d3_fake_controller.h"
#include "gm2d3_rpi_controller.h"

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <chrono>

#include <FL/fl_ask.H>

void
GM2D3::attach_controllers(void)
{
    window->manual_control->callback(static_manual_button_callback, (void *) this);

    // set bounds on history plots
    double min, max;
    for (auto &c : controllers)
    {
        min = c.second->bounds.first;
        max = c.second->bounds.second;
        window->diagnostics[c.first]->history_plot->bounds(min,max);
    }

    window->auto_control->activate();
    window->manual_control->activate();
}

void
GM2D3::detach_controllers(void)
{
    window->manual_control->deactivate();
    window->auto_control->deactivate();

    controllers.clear();
}

void
GM2D3::create_controller(Axis axis, ControllerType c_type, const Setting &c)
{
    if (c_type == ControllerType::Fake) {
        controllers[axis] = std::unique_ptr<StageController>(new FakeController(axis,
                static_encoder_state_callback, static_shutdown_callback, this, c));

#ifdef GM2D3_USE_RPI
    } else if (c_type == ControllerType::RaspberryPi) {
        controllers[axis] = std::unique_ptr<StageController>(new RaspberryPiController(axis, c));
#endif

#ifdef GM2D3_USE_GALIL
    } else if (c_type == ControllerType::Galil) {
#endif

    } else {
        debug_print(0, DebugStatementType::ERROR, "Unrecognized controller type.");
    }
}

void
GM2D3::process_config_file()
{
    try
    {
        ControllerType ct;
        const Setting& root = cfg->getRoot();
        const Setting &cs = root["controllers"];

        std::string ctype_str = root.lookup("controller_type");

        if (ctype_str == "fake") {
            ct = ControllerType::Fake;
#ifdef GM2D3_USE_RPI
        } else if (ctype_str == "raspberrypi") {
            ct = ControllerType::RaspberryPi;
#endif
#ifdef GM2D3_USE_GALIL
        } else if (ctype_str == "galil") {
            ct = ControllerType::Galil;
#endif
        } else {
            std::string unrecognized_controller_type_msg = "Unrecognized controller type: (" + ctype_str + ")";
            unrecognized_controller_type_msg += " Did you forget to enable the Raspberry Pi or Galil in CMakeLists.txt?";
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, unrecognized_controller_type_msg);
            reset();
        }

        if (cs.exists("azimuthal")) {
            create_controller(Axis::AZIMUTHAL, ct, cs["azimuthal"]);
        }
        if (cs.exists("vertical")) {
            create_controller(Axis::VERTICAL, ct, cs["vertical"]);
        }
        if (cs.exists("radial")) {
            create_controller(Axis::RADIAL, ct, cs["radial"]);
        }

        if (controllers.size() < 1) {
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, "No controllers found in config file!");
        }

        attach_controllers();
    }

    catch(const SettingNotFoundException &nfex)
    {
        debug_print(0, DebugStatementType::ERROR, "setting not found!");
        reset();
        return;
    }

    catch(const SettingTypeException &tex)
    {
        debug_print(0, DebugStatementType::ERROR, "setting of wrong type! " + std::string(tex.getPath()) );
        reset();
        return;
    }

    catch(const GM2D3Exception &gex)
    {
        debug_print(gex);
        reset();
        return;
    }

    catch(...)
    {
        debug_print(0, DebugStatementType::WARNING, "Uknown error encountered while loading config file, resetting...");
        reset();
        return;
    }

    gm2d3_state = OperatingState::UNCALIBRATED;
    window->options->config_loader->flash_config_path(FL_GREEN);
    debug_print(1, DebugStatementType::SUCCESS, "Successfully attached controllers");
}

void GM2D3::reset(void)
{
    gm2d3_state = OperatingState::RESETTING;
    debug_print(1, DebugStatementType::ATTEMPT, "Attempting reset...");

    for (auto &c : controllers) { c.second->shutdown(); }

    if (keep_updating_plots) disable_plots();
    if (keep_updating_indicators) disable_indicators();

    detach_controllers();

    Fl::lock();
    window->options->enable_history_plot->value(0);
    window->options->enable_indicators->value(0);
    window->options->enable_info->value(0);
    window->options->config_loader->path_display->value(nullptr);
    Fl::awake();
    Fl::unlock();

    cfg.reset(nullptr);

    gm2d3_state = OperatingState::DETACHED;
    debug_print(1, DebugStatementType::SUCCESS, "reset compeleted.");
}

void
GM2D3::static_manual_button_callback(Fl_Widget *button, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->manual_button_callback(button);
}

void
GM2D3::manual_button_callback(Fl_Widget *button)
{
    GM2D3ManualControlButton *b = (GM2D3ManualControlButton*) button;
    if (gm2d3_state != OperatingState::DETACHED) controllers[b->axis]->change_motor_state(b->motor_state);
}

void
GM2D3::static_kill_button_callback(Fl_Widget *button, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->kill_button_callback();
}

void
GM2D3::kill_button_callback()
{
    reset();
}

void
GM2D3::static_enable_plot_callback(Fl_Widget *enable_plot_checkbox, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->enable_plot_callback(enable_plot_checkbox);
}

void
GM2D3::static_enable_indicators_callback(Fl_Widget *enable_indicators_checkbox, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->enable_indicators_callback(enable_indicators_checkbox);
}

void
GM2D3::static_enable_info_callback(Fl_Widget *enable_info_button, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->enable_info_callback(enable_info_button);
}

void
GM2D3::static_exit_window_callback(Fl_Widget *gm2d3_window, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->exit_window_callback(gm2d3_window);
}

void
GM2D3::exit_window_callback(Fl_Widget *gm2d3_window)
{
    reset();
    Fl::lock();
    Fl::awake(exit_gm2d3, (void *) this);
    Fl::unlock();
}

void
GM2D3::enable_plot_callback(Fl_Widget *enable_plot_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_plot_checkbox;

    switch (b->value())
    {
        case 0:
            disable_plots();
            break;

        case 1:
            enable_plots();
            break;
    }
}

void
GM2D3::enable_indicators_callback(Fl_Widget *enable_indicators_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_indicators_checkbox;

    switch (b->value())
    {
        case 0:
            disable_indicators();
            break;

        case 1:
            enable_indicators();
            break;
    }
}

void
GM2D3::enable_info_callback(Fl_Widget *enable_info_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_info_checkbox;

    switch (b->value())
    {
        case 0:
            disable_info();
            break;

        case 1:
            enable_info();
            break;
    }
}

void
GM2D3::enable_info()
{
    keep_updating_info = true;
}

void
GM2D3::disable_info()
{
    keep_updating_info = false;
}

void
GM2D3::enable_indicators()
{
    debug_print(3, DebugStatementType::GENERIC, "Enabling encoder indicator icons...");

    keep_updating_indicators = true;
    for (auto &c : controllers) {
        for (auto &e : c.second->get_encoder_state()) {
            window->diagnostics[c.first]->indicators->set_dial_state(e.first, e.second);
        }
        window->diagnostics[c.first]->indicators->enable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Successfully enabled encoder indicators.");
}

void
GM2D3::disable_indicators()
{
    debug_print(3, DebugStatementType::GENERIC, "Disabling encoder indicator icons...");

    keep_updating_indicators = false;
    for (auto &c : controllers) { window->diagnostics[c.first]->indicators->disable(); }

    debug_print(3, DebugStatementType::SUCCESS, "Successfully disabled encoder indicators.");
}

void
GM2D3::enable_plots(void)
{
    debug_print(3, DebugStatementType::GENERIC, "Enabling stage position plots...");

    keep_updating_plots = true;

    for (auto &c : controllers) {
        window->diagnostics[c.first]->history_plot->enable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Successfully enabled plots.");
}

void
GM2D3::disable_plots(void)
{
    debug_print(3, DebugStatementType::ATTEMPT, "Disabling Plots..");
    keep_updating_plots = false;

    for (auto &c : controllers) {
        window->diagnostics[c.first]->history_plot->disable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Finished cleaning up plot threads.");
}

void
GM2D3::static_encoder_state_callback(Axis a, Encoder e, bool state,
        high_resolution_clock::time_point tp, const void *gm2d3)
{
    ((GM2D3 *) gm2d3)->encoder_state_callback(a,e,state, tp);
}

void
GM2D3::encoder_state_callback(Axis a, Encoder e, bool state, high_resolution_clock::time_point tp)
{
    if (gm2d3_state == OperatingState::RESETTING) {
        return;
    }
    // } else if (gm2d3_state

    // if (gm2d3_state

    if (keep_updating_indicators) {
        window->diagnostics[a]->indicators->set_dial_state(e, state);
    }

    if (keep_updating_plots) {
        if (e == Encoder::A || e == Encoder::B) {
            window->diagnostics[a]->history_plot->add_point(controllers[a]->get_current_position());
        }
    }
}

void
GM2D3::static_shutdown_callback(Axis a, const void *gm2d3)
{
    ((GM2D3 *) gm2d3)->shutdown_callback(a);
}

void
GM2D3::shutdown_callback(Axis a)
{
}

void
GM2D3::static_load_config_callback(Fl_Widget *, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->load_config_callback(nullptr);
}

void
GM2D3::load_config_callback(Fl_Widget *)
{
    std::string config_path;
    std::unique_ptr<Config> try_cfg(new Config());

    if (window->options->config_loader->user_select_config(config_path) == 0)
    {
        try {
            try_cfg->readFile(config_path.c_str());
        }

        catch(const FileIOException &fioex)
        {
            debug_print(0, DebugStatementType::ERROR, "File I/O error encountered when attempting to read configuration file: " + config_path);
            window->options->config_loader->set_path_display_color(FL_RED);
            return;
        }

        catch(const ParseException &pex)
        {
            std::string err_msg = "Parse error in configuration file: (";
            err_msg += pex.getFile();
            err_msg += ") at line: ";
            err_msg += std::to_string(pex.getLine());
            err_msg += " (";
            err_msg += pex.getError();
            err_msg += ")";

            debug_print(0, DebugStatementType::ERROR, err_msg);
            window->options->config_loader->set_path_display_color(FL_RED);
            return;
        }

        if (cfg) reset(); // unload current configuration, if present
        cfg = std::move(try_cfg);
        debug_print(0, DebugStatementType::SUCCESS, "Successfully parsed config file: " + config_path);
        process_config_file();
    }
}

GM2D3::GM2D3(int window_width, int window_height)
    : gm2d3_state(OperatingState::DETACHED),
    keep_updating_plots(false),
    keep_updating_indicators(false),
    keep_updating_info(false)
{
    window = std::unique_ptr<GM2D3Window>(new GM2D3Window(window_width,window_height,"GM2D3"));
    window->end();
    window->show();

    window->options->config_loader->callback(static_load_config_callback, (void *) this);
    window->options->enable_history_plot->callback(static_enable_plot_callback, (void *) this);
    window->options->enable_indicators->callback(static_enable_indicators_callback, (void *) this);
    window->auto_control->kill_button->callback(static_kill_button_callback, (void *) this);
    window->callback(static_exit_window_callback, (void *) this);
}

void exit_gm2d3(void *gm2d3)
{
    debug_print(1, DebugStatementType::ATTEMPT, "EXITING...");

    Fl::lock();
    ((GM2D3 *) gm2d3)->window->hide();
    Fl::flush();

    Fl::unlock();
}

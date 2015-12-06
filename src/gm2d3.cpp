#include "gm2d3.h"

#include "gm2d3_util.h"
#include "gm2d3_fltk_util.h"
#include "gm2d3_fake_controller.h"
#include "gm2d3_rpi_controller.h"

#include <map>
#include <set>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>
#include <chrono>
#include <functional>

#include <FL/fl_ask.H>

void
GM2D3::detach_controllers(void)
{
    for (const auto &c : controllers_)
    {
        window->diagnostics[c.first]->history_plot->clear();
    }

    controllers_.clear();
}

void
GM2D3::attach_controller(const ControllerFrame &frame)
{
    auto cb = [this, frame](Encoder e, bool s) -> void {
        this->encoder_transition_callback(frame.axis, e, s);
    };

    std::unique_ptr<StageController> sc;

    if (frame.controller_type == ControllerType::Fake)
    {
        sc.reset(new FakeController(frame.axis, cb, frame.c));
    }
#ifdef GM2D3_USE_RPI
    else if (frame.controller_type == ControllerType::RaspberryPi)
    {
        sc.reset(new RaspberryPiController(frame.axis, cb, frame.c));
    }
#endif

#ifdef GM2D3_USE_GALIL
    else if (frame.controller_type == ControllerType::Galil)
    {
    }
#endif

    else
    {
        std::ostringstream err;
        std::string ct_str = controller_type_to_string(frame.controller_type);
        err << "Encountered unrecognized controller type (";
        err << ct_str << ") in section where this should never happen.";
        throw make_gm2d3_exception(GM2D3Exception::Type::Programmer, err.str());
    }

    controllers_[frame.axis] = std::move(sc);

    double min, max;
    min = controllers_[frame.axis]->get_bounds().first;
    max = controllers_[frame.axis]->get_bounds().second;
    window->diagnostics[frame.axis]->history_plot->bounds(min,max);
}

void
GM2D3::process_config_file()
{
    try
    {
        gm2d3_state = OperatingState::PROCESSING_CONFIG;

        const Setting& root = cfg->getRoot();
        const Setting &cs = root["controllers"];

        std::string ctype_str = root.lookup("controller_type");

        Axis a;
        ControllerType ct;
        if (ctype_str == "fake") { ct = ControllerType::Fake; }

#ifdef GM2D3_USE_RPI
        else if (ctype_str == "raspberrypi") { ct = ControllerType::RaspberryPi; }
#endif

#ifdef GM2D3_USE_GALIL
        else if (ctype_str == "galil") { ct = ControllerType::Galil; }
#endif

        else // unrecognized controller type
        {
            std::string err_msg;
            err_msg = "Unrecognized controller type found in config: \"";
            err_msg += ctype_str + "\".";
            err_msg += " Did you forget to enable the Raspberry Pi or Galil in CMakeLists.txt?";
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, err_msg);
        }

        if (cs.exists("azimuthal"))
        {
            a = Axis::AZIMUTHAL;
            attach_controller({ .axis = a, .controller_type = ct, .c = cs["azimuthal"] });
        }

        if (cs.exists("vertical"))
        {
            a = Axis::VERTICAL;
            attach_controller({ .axis = a, .controller_type = ct, .c = cs["vertical"] });
        }

        if (cs.exists("radial"))
        {
            a = Axis::RADIAL;
            attach_controller({ .axis = a, .controller_type = ct, .c = cs["radial"] });
        }

        if (controllers_.size() < 1)
        {
            throw make_gm2d3_exception(GM2D3Exception::Type::Config, "No controllers found in config file!");
        }
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

    std::set<Axis> axes;
    for (auto const &c : controllers_) { axes.insert(c.first); }
    window->auto_control->activate(axes);
    window->manual_control->activate(axes);

    window->options->activate();

    gm2d3_state = OperatingState::UNCALIBRATED;
    window->options->config_loader->flash_config_path(FL_GREEN);

    debug_print(1, DebugStatementType::SUCCESS, "Successfully attached controllers");
}

void GM2D3::reset(void)
{
    if (gm2d3_state == OperatingState::DETACHED) return;

    gm2d3_state = OperatingState::RESETTING;
    debug_print(1, DebugStatementType::ATTEMPT, "Attempting reset...");

    if (keep_updating_plots_) disable_plots();
    if (keep_updating_indicators_) disable_indicators();
    if (keep_updating_info_) disable_info();

    for (auto &c : controllers_) { c.second->shutdown(); }

    detach_controllers();

    window->reset();

    cfg.reset(nullptr);

    gm2d3_state = OperatingState::DETACHED;
    debug_print(1, DebugStatementType::SUCCESS, "reset compeleted.");
}

void
GM2D3::manual_button_callback(Fl_Widget *button)
{
    GM2D3ManualControlButton *b = (GM2D3ManualControlButton*) button;
    if (gm2d3_state != OperatingState::DETACHED)
    {
        controllers_[b->axis]->change_motor_state(b->motor_state);
    }
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
GM2D3::enable_plot_checkbox_callback(Fl_Widget *enable_plot_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_plot_checkbox;

    switch (b->value())
    {
    case 0:
        disable_plots();
        break;

    case 1:
        enable_plots();
        for (auto const &c : controllers_) { update_plot(c.first); }
        break;
    }
}

void
GM2D3::enable_indicators_checkbox_callback(Fl_Widget *enable_indicators_checkbox)
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
GM2D3::enable_info_checkbox_callback(Fl_Widget *enable_info_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_info_checkbox;

    switch (b->value())
    {
    case 0:
        disable_info();
        break;

    case 1:
        enable_info();
        for (auto const &c : controllers_) { update_info(c.first); }
        break;
    }
}

void
GM2D3::enable_info()
{
    keep_updating_info_ = true;
    for (auto const &c : controllers_)
    {
        window->diagnostics[c.first]->info->activate();
        update_info(c.first);
    }
}

void
GM2D3::disable_info()
{
    keep_updating_info_ = false;
    for (auto const &c : controllers_)
    {
        window->diagnostics[c.first]->info->value("");
        window->diagnostics[c.first]->info->deactivate();
    }
}

void
GM2D3::enable_indicators()
{
    debug_print(3, DebugStatementType::GENERIC, "Enabling encoder indicator icons...");

    keep_updating_indicators_ = true;
    for (auto const &c : controllers_)
    {
        for (auto &e : c.second->get_encoder_state())
        {
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

    keep_updating_indicators_ = false;
    for (auto const &c : controllers_)
    {
        window->diagnostics[c.first]->indicators->disable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Successfully disabled encoder indicators.");
}

void
GM2D3::enable_plots(void)
{
    debug_print(3, DebugStatementType::GENERIC, "Enabling stage position plots...");

    keep_updating_plots_ = true;

    for (auto const &c : controllers_)
    {
        window->diagnostics[c.first]->history_plot->enable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Successfully enabled plots.");
}

void
GM2D3::disable_plots(void)
{
    debug_print(3, DebugStatementType::ATTEMPT, "Disabling Plots..");
    keep_updating_plots_ = false;

    for (auto const &c : controllers_)
    {
        window->diagnostics[c.first]->history_plot->disable();
    }

    debug_print(3, DebugStatementType::SUCCESS, "Finished cleaning up plot threads.");
}

void
GM2D3::encoder_transition_callback(Axis a, Encoder e, bool state)
{
    if (gm2d3_state == OperatingState::RESETTING)
    {
        return;
    }

    if (keep_updating_indicators_)
    {
        update_indicator(a,e,state);
    }

    if (keep_updating_plots_ & (e == Encoder::A || e == Encoder::B))
    {
        update_plot(a);
    }

    if (keep_updating_info_)
    {
        update_info(a);
    }
}

void
GM2D3::update_plot(Axis a)
{
    double cur_pos = controllers_[a]->get_current_position();
    window->diagnostics[a]->history_plot->add_point(cur_pos);
}

void
GM2D3::update_indicator(Axis a, Encoder e, bool s)
{
    window->diagnostics[a]->indicators->set_dial_state(e, s);
}

void
GM2D3::update_info(Axis a)
{
    std::ostringstream info_str;
    info_str << "RESOLUTION: " << controllers_[a]->get_resolution() << std::endl;
    window->diagnostics[a]->info->value(info_str.str().c_str());
}

void
GM2D3::load_config_callback()
{
    std::string config_path;
    std::unique_ptr<Config> try_cfg(new Config());

    if (window->options->config_loader->user_select_config(config_path) == 0)
    {
        try
        {
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
      keep_updating_plots_(false),
      keep_updating_indicators_(false),
      keep_updating_info_(false)
{
    window = std::unique_ptr<GM2D3Window>(new GM2D3Window(window_width,window_height,"GM2D3"));

    FLTK_CALLBACK(this, &GM2D3::exit_window_callback, window);
    FLTK_CALLBACK(this, &GM2D3::load_config_callback, window->options->config_loader->open_button);
    FLTK_CALLBACK(this, &GM2D3::enable_plot_checkbox_callback, window->options->enable_plot_checkbox);
    FLTK_CALLBACK(this, &GM2D3::enable_indicators_checkbox_callback, window->options->enable_indicators_checkbox);
    FLTK_CALLBACK(this, &GM2D3::enable_info_checkbox_callback, window->options->enable_info_checkbox);
    FLTK_CALLBACK(this, &GM2D3::reset, window->auto_control->kill_button);

    for (auto const &a : ALL_AXES)
    {
        for (auto const &s : ALL_MOTOR_STATES)
        {
            FLTK_CALLBACK(this, &GM2D3::manual_button_callback, window->manual_control->buttons[a][s]);
        }
    }
}

void exit_gm2d3(void *gm2d3)
{
    debug_print(1, DebugStatementType::ATTEMPT, "EXITING...");

    Fl::lock();
    ((GM2D3 *) gm2d3)->window->hide();
    Fl::flush();

    Fl::unlock();
}

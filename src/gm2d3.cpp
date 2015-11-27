#include "gm2d3.h"

#include "gm2d3_util.h"
#include "gm2d3_fake_controller.h"
#include "gm2d3_rpi_controller.h"

#include <map>
#include <string>
#include <memory>
#include <chrono>

#include <FL/fl_ask.H>

void
GM2D3::attach_controllers(void)
{
     window->manual_control->callback(static_manual_button_callback, (void *) this);

    double min, max;
    for (auto &c : controllers)
    {
        min = c.second->bounds.first;
        max = c.second->bounds.second;
        window->diagnostics[c.first]->history_plot->bounds(min,max);
    }

    window->auto_control->activate();
    for (auto &c : controllers)
    {
        window->auto_control->enable_input(c.first);
        window->manual_control->enable_axis(c.first);
    }
}

void
GM2D3::create_controller(Axis axis, ControllerType ct, const Setting &c)
{
    if (ct == ControllerType::Fake) {
        controllers[axis] = std::shared_ptr<StageController>(new FakeController(axis,
                    static_encoder_state_callback, static_shutdown_callback, this, c));

#ifdef GM2D3_USE_RPI
    } else if (ct == ControllerType::RaspberryPi) {
        controllers[axis] = std::shared_ptr<StageController>(new RaspberryPiController(axis, c));
#endif

#ifdef GM2D3_USE_GALIL
    } else if (ct == ControllerType::Galil) {
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

    catch(const GM2D3Exception &cex)
    {
        debug_print(0, DebugStatementType::ERROR, cex.msg);
        reset();
        return;
    }

    catch(...)
    {
        debug_print(0, DebugStatementType::WARNING, "Uknown error encountered, resetting just to be safe...");
        reset();
        return;
    }

    gm2d3_state = OperatingState::WAITING_ON_USER;

    window->options->config_loader->flash_config_path(FL_GREEN);
    debug_print(1, DebugStatementType::SUCCESS, "Successfully attached controllers");
}

// TODO: void all stage-related callbacks
void GM2D3::reset(void)
{
    disable_indicators();
    cleanup_plot_threads();

    for (auto &c : controllers)
    {
        window->diagnostics[c.first]->indicators->disable();
        window->diagnostics[c.first]->history_plot->disable();
        c.second->shutdown();
    }


    controllers.clear();
    cfg = nullptr;
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
    controllers[b->axis]->change_motor_state(b->motor_state);
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
GM2D3::static_exit_window_callback(Fl_Widget *gm2d3_window, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->exit_window_callback(gm2d3_window);
}

void
GM2D3::exit_window_callback(Fl_Widget *gm2d3_window)
{
     if (fl_choice("Really Exit?", "NO", "YES", nullptr)) {
         reset();
         window->hide(); 
     }
}

void
GM2D3::enable_plot_callback(Fl_Widget *enable_plot_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_plot_checkbox;

    switch (b->value())
    {
        case 0:
            cleanup_plot_threads();
            break;

        case 1:
            start_plot_threads();
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
GM2D3::enable_indicators()
{

    keep_updating_indicators = true;
    for (const auto &c : controllers) {
        for (const auto &e : c.second->get_encoder_state()) {
            window->diagnostics[c.first]->indicators->set_dial_state(e.first, e.second);
        }
        window->diagnostics[c.first]->indicators->enable();
    }
}

void
GM2D3::disable_indicators()
{
    keep_updating_indicators = false;
    for (const auto &c : controllers) {
        for (const auto &e : ALL_ENCODERS) {
            window->diagnostics[c.first]->indicators->set_dial_state(e, false);
        }
        window->diagnostics[c.first]->indicators->disable();
    }
}

void
GM2D3::start_plot_threads(void)
{
    *keep_updating_plots = true;
    for (auto &c : controllers) {
        window->diagnostics[c.first]->history_plot->enable();
    }

    auto plot_updater = [](
            std::shared_ptr<StageController> const &sc,
            std::shared_ptr<GM2D3StageHistoryPlot> hp,
            std::shared_ptr<bool> const &kp) -> void
    {
        while(*kp && sc != nullptr)
        {
            Fl::lock();
            hp->add_point(sc->get_current_position());
            Fl::awake();
            Fl::unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    for (auto& c : controllers)
    {
        plot_threads.push_back(std::thread(plot_updater, c.second,
                window->diagnostics[c.first]->history_plot, keep_updating_plots));
    }
}

void
GM2D3::cleanup_plot_threads(void)
{
    *keep_updating_plots = false;
    for (auto &c : controllers) {
        window->diagnostics[c.first]->history_plot->disable();
    }

    while (!plot_threads.empty())
    {
        plot_threads.back().join();
        plot_threads.pop_back();
    }
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
    if (keep_updating_indicators) {
        window->diagnostics[a]->indicators->set_dial_state(e, state);
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

void
GM2D3::update_stats(void)
{

}

GM2D3::GM2D3(int window_width, int window_height)
    : gm2d3_state(OperatingState::DETACHED),
    keep_updating_indicators(false)
{
    window = std::unique_ptr<GM2D3Window>(new GM2D3Window(window_width,window_height,"GM2D3"));
    window->end();
    window->show();

    keep_updating_plots      = std::make_shared<bool>();
    *keep_updating_plots     = false;

    window->options->config_loader->callback(static_load_config_callback, (void *) this);
    window->options->enable_history_plot->callback(static_enable_plot_callback, (void *) this);
    window->options->enable_indicators->callback(static_enable_indicators_callback, (void *) this);
    window->callback(static_exit_window_callback, (void *) this);
}

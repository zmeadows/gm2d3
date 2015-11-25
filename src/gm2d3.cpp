#include <map>

#include "gm2d3.h"
#include "gm2d3_util.h"
#include "gm2d3_fake_controller.h"
#include "gm2d3_rpi_controller.h"

#include <string>

void
GM2D3::setup_controllers(void)
{
     window->manual_control->callback(static_manual_button_callback, (void *) this);

    double min, max;
    for (auto &c : controllers)
    {
        min = c.second->bounds.first;
        max = c.second->bounds.second;
        window->diagnostics[c.first]->history_plot->bounds(min,max);
        std::cout << min << " " << max << std::endl;
    }

    window->auto_control->activate();
    for (auto &c : controllers)
    {
        window->auto_control->enable_input(c.first);
        window->manual_control->enable_axis(c.first);
    }
}

void
GM2D3::attach_controller(Axis axis, ControllerType ct, const Setting &c)
{
    if (ct == ControllerType::Fake) {
        controllers[axis] = std::shared_ptr<StageController>(new FakeController(axis,
                    static_encoder_state_callback, this, c));

#ifdef GM2D3_USE_RPI
    } else if (ct == ControllerType::RaspberryPi) {
        controllers[axis] = std::shared_ptr<StageController>(new RaspberryPiController(axis, c));
#endif

#ifdef GM2D3_USE_GALIL
    } else if (ct == ControllerType::Galil) {
#endif

    } else {
        debug_print(0, "ERROR: unrecognized controller type");
    }

}

bool
GM2D3::process_config_file()
{
    unprocess_config_file();

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
            debug_print(0, "ERROR: unrecognized controller type");
            return false;
        }

        if (cs.exists("azimuthal")) {
            attach_controller(Axis::AZIMUTHAL, ct, cs["azimuthal"]);
        }
        if (cs.exists("vertical")) {
            attach_controller(Axis::VERTICAL, ct, cs["vertical"]);
        }
        if (cs.exists("radial")) {
            attach_controller(Axis::RADIAL, ct, cs["radial"]);
        }

        if (controllers.size() < 1) { return false; }

        setup_controllers();
    }

    catch(const SettingNotFoundException &nfex)
    {
        debug_print(0, "setting not found!");
        return false;
    }

    catch(const SettingTypeException &tex)
    {
        debug_print(0, "setting of wrong type! " + std::string(tex.getPath()) );
        return false;
    }

    catch(const ControllerException &cex)
    {
        debug_print(0, cex.msg);
        return false;
    }



    window->options->config_loader->flash_config_path(FL_GREEN);
    debug_print(1, "Successfully processed config file");
    return true;
}

// TODO: void all callbacks
void GM2D3::unprocess_config_file(void)
{
    controllers.clear();
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
GM2D3::enable_plot_callback(Fl_Widget *enable_plot_checkbox)
{
    Fl_Check_Button *b = (Fl_Check_Button*) enable_plot_checkbox;

    switch (b->value())
    {
        case 0:
            *keep_updating_plots = false;
            for (auto &c : controllers) {
                window->diagnostics[c.first]->history_plot->disable();
            }
            break;
        case 1:
            *keep_updating_plots = true;
            for (auto &c : controllers) {
                window->diagnostics[c.first]->history_plot->enable();
            }
            detach_plot_threads();
            break;
    }
}

void
GM2D3::detach_plot_threads(void)
{
    auto plot_updater = [](
            std::shared_ptr<StageController> const &sc,
            std::shared_ptr<GM2D3StageHistoryPlot> hp,
            std::shared_ptr<bool> const &kp) -> void
    {
        while(*kp)
        {
            Fl::lock();
            hp->add_point(sc->get_current_position());
            Fl::awake();
            Fl::unlock();
            usleep(1e6);
        }
    };

    for (auto& c : controllers)
    {
        std::thread plot_thread(plot_updater, c.second,
                window->diagnostics[c.first]->history_plot, keep_updating_plots);
        plot_thread.detach();
    }
}


void
GM2D3::static_encoder_state_callback(Axis a, Encoder e, bool state, const void *gm2d3)
{
    ((GM2D3 *) gm2d3)->encoder_state_callback(a,e,state);
}

void
GM2D3::encoder_state_callback(Axis a, Encoder e, bool state)
{
    window->diagnostics[a]->indicators->set_dial_state(e, state);
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

    if (window->options->config_loader->user_select_config(config_path) == 0)
    {
        try {
            cfg = std::unique_ptr<Config>(new Config());
            cfg->readFile(config_path.c_str());
        }

        catch(const FileIOException &fioex)
        {
            debug_print(1, "File I/O error encountered when attempting to read configuration file: " + config_path);
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

            debug_print(1, err_msg);
            window->options->config_loader->set_path_display_color(FL_RED);
            return;
        }

        debug_print(1, "Successfully parsed config file: " + config_path);

        process_config_file();
    }
}

GM2D3::GM2D3(int window_width, int window_height)
{
    window = std::unique_ptr<GM2D3Window>(new GM2D3Window(window_width,window_height,"GM2D3"));
    window->end();
    window->show();

    keep_updating_plots      = std::make_shared<bool>();
    *keep_updating_plots     = false;
    keep_updating_indicators = false;
    keep_updating_stats      = false;

    window->options->config_loader->callback(static_load_config_callback, (void *) this);
    window->options->enable_history_plot->callback(static_enable_plot_callback, (void *) this);

}

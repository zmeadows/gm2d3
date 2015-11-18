#include <map>

#include "gm2d3.h"
#include "gm2d3_util.h"
#include "gm2d3_fake_controller.h"

#include <string>

void
GM2D3::setup_callbacks(ControllerType ct)
{
    switch (ct) {
        case ControllerType::Fake:
            setup_fake_callbacks();
            break;
        case ControllerType::RaspberryPi:
            setup_rpi_callbacks();
            break;
        case ControllerType::Galil:
            setup_galil_callbacks();
            break;
        default:
            break;
    }
}

void
GM2D3::setup_fake_callbacks(void)
{
}

void
GM2D3::setup_rpi_callbacks(void)
{
}

void
GM2D3::setup_galil_callbacks(void)
{
}

void
GM2D3::attach_controller(Axis axis, ControllerType ct, const Setting &c)
{
    if (ct == ControllerType::Fake) {
        controllers[axis] = std::unique_ptr<StageController>(new FakeController(c));
    } else if (ct == ControllerType::RaspberryPi) {
    } else if (ct == ControllerType::Galil) {
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
        } else if (ctype_str == "rapsberrypi") {
            ct = ControllerType::RaspberryPi;
        } else if (ctype_str == "galil") {
            ct = ControllerType::Galil;
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

        setup_callbacks(ct);
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
    std::cout << controllers.size() << std::endl;
    std::cout << controllers[Axis::AZIMUTHAL]->bounds.second << std::endl;
    return true;
}

// TODO: void all callbacks
void GM2D3::unprocess_config_file(void)
{
    controllers.clear();
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

    controllers[Axis::AZIMUTHAL] = nullptr;
    controllers[Axis::VERTICAL]  = nullptr;
    controllers[Axis::RADIAL]    = nullptr;

    window->options->config_loader->callback(static_load_config_callback, (void *) this);
}

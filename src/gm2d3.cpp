#include <map>

#include "gm2d3.h"
#include "gm2d3_util.h"


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
        try { cfg->readFile(config_path.c_str()); }

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
        window->options->config_loader->set_path_display_color(FL_WHITE);
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

    cfg = std::unique_ptr<Config>(new Config());
    window->options->config_loader->callback(static_load_config_callback, (void *) this);
}

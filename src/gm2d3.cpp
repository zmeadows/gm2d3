#include <map>

#include "gm2d3.h"

void
GM2D3::static_load_config_callback(Fl_Widget *, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->load_config_callback(nullptr);
}

void
GM2D3::load_config_callback(Fl_Widget *)
{
    std::string config_path;

    if (window->options->config_loader->get_config_path(config_path) == 0)
    {
        std::cout << config_path << std::endl;
        cfg->readFile(config_path.c_str());
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

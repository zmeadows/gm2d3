#include <map>

#include "gm2d3.h"
#include "gm2d3_stage.h"

void
GM2D3::static_load_config_callback(Fl_Widget *_config_loader, void *gm2d3)
{
    ((GM2D3 *) gm2d3)->load_config_callback(_config_loader);
}

void
GM2D3::load_config_callback(Fl_Widget *_open_button)
{
    std::cout << window->options->config_loader->load_config_file() << std::endl;
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

void GM2D3::attach_controller(std::unique_ptr<RaspberryPiController> c)
{
    controllers[c->axis] = std::move(c);
}

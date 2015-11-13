#include <map>

#include "gm2d3.h"
#include "gm2d3_stage.h"

GM2D3::GM2D3(int window_width, int window_height)
{
    window = new GM2D3Window(window_width,window_height,"GM2D3");

    controllers[Axis::AZIMUTHAL] = NULL;
    controllers[Axis::VERTICAL]  = NULL;
    controllers[Axis::RADIAL]    = NULL;
}

void GM2D3::attach_controller(StageController *controller)
{
    controllers[controller->axis] = controller;
}

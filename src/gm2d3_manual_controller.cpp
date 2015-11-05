#include "gm2d3_manual_controller.h"

GM2D3ManualController::GM2D3ManualController(int x, int y, int w, int h, const char *label) :
    Fl_Pack(x,y,w,h,label)
{
    type(Fl_Pack::HORIZONTAL);
    align(FL_ALIGN_LEFT);

    ccw = new Fl_Light_Button(0, 0, 100,30,"CCW");
    cw = new Fl_Light_Button(10, 0, 100,30,"CW");
    ccw->align(FL_ALIGN_CENTER);
    cw->align(FL_ALIGN_CENTER);

    end();
}

GM2D3ManualController::~GM2D3ManualController() {}

GM2D3ManualControllerGroup::GM2D3ManualControllerGroup(int x, int y, int w, int h, const char *label) :
    Fl_Pack(x,y,w,h,label)
{
    align(FL_ALIGN_TOP_LEFT);
    box(FL_NO_BOX);

    acm = new GM2D3ManualController(0,0,250,30, "X: ");
    vcm = new GM2D3ManualController(0,10,250,30,"Y: ");
    rcm = new GM2D3ManualController(0,20,250,30,"Z: ");
    end();
}


GM2D3ManualControllerGroup::~GM2D3ManualControllerGroup() {}

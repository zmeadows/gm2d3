#ifndef GM2D3_MANUAL_CONTROLLER
#define GM2D3_MANUAL_CONTROLLER

#include <Fl/Enumerations.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Light_Button.H>

#include "gm2d3_stage.h"

class GM2D3ManualController : public Fl_Pack {
    public:
        GM2D3ManualController(int x, int y, int w, int h, const char* label);
        virtual ~GM2D3ManualController();

    private:
        Fl_Light_Button *cw, *ccw;
};

class GM2D3ManualControllerGroup : public Fl_Pack {
    public:
        GM2D3ManualControllerGroup(int x, int y, int w, int h, const char* label);
        virtual ~GM2D3ManualControllerGroup();

    private:
        GM2D3ManualController *acm, *vcm, *rcm;

};

#endif

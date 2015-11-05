#include "gm2d3_auto_controller.h"

GM2D3AutoController::GM2D3AutoController(int x, int y, int w, int h, const char* label) :
    Fl_Input(x,y,w,h,label)
{
    align(FL_ALIGN_LEFT);
}

GM2D3AutoController::~GM2D3AutoController() {}

GM2D3AutoControllerGroup::GM2D3AutoControllerGroup(int x, int y, int w, int h, const char* label) :
    Fl_Pack(x,y,w,h,label)
{
    align(FL_ALIGN_TOP_LEFT);
    type(Fl_Pack::HORIZONTAL);
    spacing(30);

    {Fl_Pack *inputBoxes = new Fl_Pack(0,0,125,h);
        aca = new GM2D3AutoController(0,0,125,30, "AZIMUTHAL: ");
        vca = new GM2D3AutoController(0,10,125,30, "VERTICAL: ");
        rca = new GM2D3AutoController(0,20,125,30, "RADIAL: ");
        inputBoxes->end();
    }

    {Fl_Pack *inputButtons = new Fl_Pack(10,0,125,h);
        calibrate_button = new Fl_Light_Button(0,0,125,20, "CALIBRATE");
        calibrate_button->align(FL_ALIGN_CENTER);

        go_button = new Fl_Light_Button(0,0,125,20, "GO");
        go_button->align(FL_ALIGN_CENTER);

        stop_button = new Fl_Button(0,0,125,20,"STOP");
        stop_button->color(FL_RED);

        inputButtons->end();
    }

    end();
}

GM2D3AutoControllerGroup::~GM2D3AutoControllerGroup() {}

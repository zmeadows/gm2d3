#include "gm2d3_window.h"

#include <Fl/Fl_Pack.H>

GM2D3Window::GM2D3Window(int w, int h, const char* t) :
    Fl_Double_Window(w, h, t)
{
    {Fl_Pack *vertical_pack = new Fl_Pack(45,30,750,870);
        vertical_pack->type(Fl_Pack::VERTICAL);
        vertical_pack->spacing(40);
        {Fl_Pack *top_row = new Fl_Pack(0,0,500,4*40);
            top_row->type(Fl_Pack::HORIZONTAL);
            top_row->spacing(200);
            manual_controller = new GM2D3ManualControllerGroup(0,0,250, 3*40, "MANUAL CONTROL:");
            auto_controller = new GM2D3AutoControllerGroup(10,0,250, 3*40, "AUTO CONTROL:");
            top_row->end();
        }

        ad = new GM2D3StageDiagnostics(0,100,w-60,150,"Azimuthal Stage History");
        vd = new GM2D3StageDiagnostics(0,300,w-60,150,"Vertical Stage History");
        rd = new GM2D3StageDiagnostics(0,500,w-60,150,"Radial Stage History");
        vertical_pack->end();
    }
    end();
}

GM2D3Window::~GM2D3Window() {}

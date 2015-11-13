#ifndef GM2D3_WINDOW
#define GM2D3_WINDOW

#include <Fl/Fl_Double_Window.H>

#include "gm2d3_manual_controller.h"
#include "gm2d3_auto_controller.h"
#include "gm2d3_stage_diagnostics.h"

class GM2D3Window : public Fl_Double_Window {
    public:
        GM2D3Window(int w, int h, const char* t);
        virtual ~GM2D3Window();

    private:
        GM2D3ManualControllerGroup *manual_controller;
        GM2D3AutoControllerGroup *auto_controller;
        GM2D3StageDiagnostics *ad, *vd, *rd;
};

#endif

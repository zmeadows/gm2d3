#pragma once

#include <Fl/Fl_Double_Window.H>

#include <map>
#include <memory>

#include "gm2d3_manual_controller.h"
#include "gm2d3_auto_controller.h"
#include "gm2d3_stage_diagnostics.h"
#include "gm2d3_stage_controller.h"
#include "gm2d3_options.h"

class GM2D3Window : public Fl_Double_Window
{
public:
    GM2D3Window(int w, int h, const char* t);

    std::unique_ptr<GM2D3ManualControlGUI> manual_control;
    std::unique_ptr<GM2D3AutoControlGUI> auto_control;
    std::unique_ptr<GM2D3OptionsGUI> options;
    std::map<Axis, std::unique_ptr<GM2D3StageDiagnostics>> diagnostics;
};

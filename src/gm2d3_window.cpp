#include "gm2d3_window.h"

#include <Fl/Fl_Pack.H>

GM2D3Window::GM2D3Window(int w, int h, const char* t) :
    Fl_Double_Window(w, h, t)
{
    manual_control = std::unique_ptr<GM2D3ManualControlGUI>
         (new GM2D3ManualControlGUI(WINDOW_EDGE_GAP, WINDOW_EDGE_GAP, 400, 145));

    auto_control = std::unique_ptr<GM2D3AutoControlGUI>
         (new GM2D3AutoControlGUI(400 + WINDOW_EDGE_GAP + ADJACENT_SPACING, WINDOW_EDGE_GAP, 400, 145));

    options = std::unique_ptr<GM2D3OptionsGUI>
        (new GM2D3OptionsGUI(800 + WINDOW_EDGE_GAP + 2*ADJACENT_SPACING, WINDOW_EDGE_GAP, 250, 145));

    diagnostics[Axis::AZIMUTHAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,180, w-2*WINDOW_EDGE_GAP,150, Axis::AZIMUTHAL));

    diagnostics[Axis::VERTICAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,340, w-2*WINDOW_EDGE_GAP,150, Axis::VERTICAL));

    diagnostics[Axis::RADIAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,500, w-2*WINDOW_EDGE_GAP,150, Axis::RADIAL));

    color(fl_rgb_color(160,160,160));
}

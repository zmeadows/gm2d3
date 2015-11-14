#include "gm2d3_window.h"

#include <Fl/Fl_Pack.H>

GM2D3Window::GM2D3Window(int w, int h, const char* t) :
    Fl_Double_Window(w, h, t)
{
    manual_control = std::unique_ptr<GM2D3ManualControlGUI>
        (new GM2D3ManualControlGUI(WINDOW_EDGE_GAP, WINDOW_EDGE_GAP, 400, 130));

    auto_control = std::unique_ptr<GM2D3AutoControlGUI>
        (new GM2D3AutoControlGUI(580, WINDOW_EDGE_GAP, 400, 130));

    diagnostics[Axis::AZIMUTHAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,180, w-2*WINDOW_EDGE_GAP,150, "AZIMUTHAL"));

    diagnostics[Axis::AZIMUTHAL]->diagnostics_box->align(FL_ALIGN_TOP_LEFT);
    diagnostics[Axis::AZIMUTHAL]->diagnostics_box->labelfont(FL_BOLD);
    diagnostics[Axis::AZIMUTHAL]->diagnostics_box->label("DIAGNOSTICS:");

    diagnostics[Axis::VERTICAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,340, w-2*WINDOW_EDGE_GAP,150, "VERTICAL"));

    diagnostics[Axis::RADIAL] = std::unique_ptr<GM2D3StageDiagnostics>
        (new GM2D3StageDiagnostics(WINDOW_EDGE_GAP,500, w-2*WINDOW_EDGE_GAP,150, "RADIAL"));
}

GM2D3Window::~GM2D3Window() {}

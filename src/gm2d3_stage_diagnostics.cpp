#include "gm2d3_stage_diagnostics.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GM2D3StageHistoryPlot::GM2D3StageHistoryPlot(int x, int y, int w, int h, Axis _axis) :
    Fl_Chart(x,y,w,h),
    axis(_axis)
{
    type(FL_LINE_CHART);
    color(fl_rgb_color(240,240,240));
    disable();
}

void
GM2D3StageHistoryPlot::disable()
{
    deactivate();
    align(FL_ALIGN_CENTER);
    label("PAUSED");
}

void
GM2D3StageHistoryPlot::enable()
{
    activate();
    align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
    switch (axis)
    {
        case Axis::AZIMUTHAL:
            label("AZIMUTHAL");
            break;
        case Axis::VERTICAL:
            label("VERTICAL");
            break;
        case Axis::RADIAL:
            label("RADIAL");
            break;
    }
}

void GM2D3StageHistoryPlot::set_line_color(Fl_Color color) { line_color = color; }

void GM2D3StageHistoryPlot::add_point(double val)
{
    //add(val, std::to_string(val).c_str(), line_color);
    add(val, nullptr, line_color);
}

GM2D3StageHistoryPlot::~GM2D3StageHistoryPlot() {}

GM2D3StageIndicators::GM2D3StageIndicators(int x, int y, int w) :
    on_color(FL_GREEN),
    off_color(FL_WHITE)
{
    std::cout << on_color << std::endl << off_color << std::endl;
    indicator_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,w));
    indicator_box->box(FL_FLAT_BOX);
    indicator_box->color(fl_rgb_color(210,210,210));
    indicator_box->align(FL_ALIGN_TOP_LEFT);

    const int w2 = w - 2 * ADJACENT_SPACING;
    const int dial_width = 0.9 * 0.5 * w2;
    const int x_0 = x + ADJACENT_SPACING;
    const int y_0 = y + ADJACENT_SPACING;

    dials[Encoder::A] = std::unique_ptr<Fl_Dial>(new Fl_Dial(x_0,y_0,dial_width,dial_width));
    dials[Encoder::A]->type(FL_FILL_DIAL);
    dials[Encoder::A]->color(off_color);
    dials[Encoder::A]->color2(off_color);
    dials[Encoder::A]->label("A");
    dials[Encoder::A]->align(FL_ALIGN_CENTER);
    dials[Encoder::A]->labelfont(FL_BOLD);

    dials[Encoder::B] = std::unique_ptr<Fl_Dial>(new Fl_Dial(x_0 + dial_width + 0.1*w2, y_0,
            dial_width, dial_width));
    dials[Encoder::B]->type(FL_FILL_DIAL);
    dials[Encoder::B]->color(off_color);
    dials[Encoder::B]->color2(off_color);
    dials[Encoder::B]->label("B");
    dials[Encoder::B]->align(FL_ALIGN_CENTER);
    dials[Encoder::B]->labelfont(FL_BOLD);

    dials[Encoder::C] = std::unique_ptr<Fl_Dial>(new Fl_Dial(x_0, y_0 + dial_width + 0.1*w2,
            dial_width, dial_width));
    dials[Encoder::C]->type(FL_FILL_DIAL);
    dials[Encoder::C]->color(off_color);
    dials[Encoder::C]->color2(off_color);
    dials[Encoder::C]->label("C");
    dials[Encoder::C]->align(FL_ALIGN_CENTER);
    dials[Encoder::C]->labelfont(FL_BOLD);

    dials[Encoder::D] = std::unique_ptr<Fl_Dial>(new Fl_Dial(x_0 + dial_width + 0.1*w2,
            y_0 + dial_width + 0.1*w2, dial_width, dial_width));
    dials[Encoder::D]->type(FL_FILL_DIAL);
    dials[Encoder::D]->color(off_color);
    dials[Encoder::D]->color2(off_color);
    dials[Encoder::D]->label("D");
    dials[Encoder::D]->align(FL_ALIGN_CENTER);
    dials[Encoder::D]->labelfont(FL_BOLD);
}

GM2D3StageIndicators::~GM2D3StageIndicators() {}


GM2D3StageDiagnostics::GM2D3StageDiagnostics(int x, int y, int w, int h, Axis axis)
{

    diagnostics_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,h));
    diagnostics_box->box(FL_EMBOSSED_BOX);
    diagnostics_box->color(fl_rgb_color(210,210,210));

    const int w2 = w - 2*BOX_EDGE_GAP;
    const int h2 = h - 2*BOX_EDGE_GAP;
    const int x_0 = x + BOX_EDGE_GAP;
    const int y_0 = y + BOX_EDGE_GAP;

    const int info_width = h2 * 1.8;
    const int plot_width = w2 - info_width - h2 - 2*ADJACENT_SPACING;

    history_plot = std::shared_ptr<GM2D3StageHistoryPlot>
        (new GM2D3StageHistoryPlot(x_0,y_0,plot_width,h2, axis));

    info = std::unique_ptr<Fl_Multiline_Output>
        (new Fl_Multiline_Output(x_0 + plot_width + ADJACENT_SPACING,y_0, info_width, h2));

    indicators = std::unique_ptr<GM2D3StageIndicators>
        (new GM2D3StageIndicators(x_0 + plot_width + info_width + 2*ADJACENT_SPACING,y_0,h2));

}

GM2D3StageDiagnostics::~GM2D3StageDiagnostics() {

}


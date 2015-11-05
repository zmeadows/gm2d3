#include "gm2d3_stage_plot.h"

GM2D3StageDiagnostics::GM2D3StageDiagnostics(int x, int y, int w, int h, const char* label) :
    Fl_Pack(x,y,w,h)
{

    type(Fl_Pack::HORIZONTAL);
    chart = new Fl_Chart(0,0,300,h,label);
    chart->type(FL_LINE_CHART);
    diagnostics = new Fl_Multiline_Output(10,0,200,h);

    {Fl_Pack
}

GM2D3StageDiagnostics::~GM2D3StageDiagnostics() {}

void
GM2D3StageDiagnostics::set_line_color(Fl_Color color)
{ line_color = color; }

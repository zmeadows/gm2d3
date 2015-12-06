#pragma once

#include <Fl/Fl_Chart.H>
#include <Fl/Fl_Dial.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Multiline_Output.H>
#include <Fl/Fl_Box.H>

#include <map>
#include <iostream>
#include <memory>

#include "gm2d3_const.h"
#include "gm2d3_util.h"
#include "gm2d3_fltk_util.h"

#include <Fl/Fl.H>

class GM2D3StageHistoryPlot : public Fl_Chart
{
public:
    GM2D3StageHistoryPlot(int x, int y, int w, int h, Axis _axis);

    const Axis axis;

    void add_point(double val);

    void enable();
    void disable();

private:
    unsigned int n;
    double last_point_added_;
    Fl_Color line_color;
};

class GM2D3StageIndicators
{
public:
    GM2D3StageIndicators(int x, int y, int w);

    void set_dial_state(Encoder e, bool state);

    void enable();
    void disable();

private:
    std::unique_ptr<Fl_Box> indicator_box;
    std::map<Encoder, std::unique_ptr<Fl_Dial>> dials;
    Fl_Color on_color, off_color;
};

class GM2D3StageDiagnostics
{
public:
    GM2D3StageDiagnostics(int x, int y, int w, int h, Axis axis);

    std::unique_ptr<GM2D3StageHistoryPlot> history_plot;
    std::unique_ptr<GM2D3StageIndicators> indicators;
    std::unique_ptr<Fl_Multiline_Output> info;

private:
    std::unique_ptr<Fl_Box> diagnostics_box;
};

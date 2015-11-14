#ifndef GM2D3_STAGE_DIAGNOSTICS
#define GM2D3_STAGE_DIAGNOSTICS

#include <Fl/Fl_Chart.H>
#include <Fl/Fl_Dial.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Multiline_Output.H>
#include <Fl/Fl_Box.H>

#include <map>
#include <iostream>
#include <memory.h>

#include "gm2d3_stage.h"


class GM2D3StageHistoryPlot : public Fl_Chart
{
    public:
        GM2D3StageHistoryPlot(int x, int y, int w, int h);
        virtual ~GM2D3StageHistoryPlot();

        void set_line_color(Fl_Color color);
        void plot_random_points(double min, double max, int N);

    private:
        Fl_Color line_color;
};

class GM2D3StageIndicators
{
    public:
        GM2D3StageIndicators(int x, int y, int w);
        virtual ~GM2D3StageIndicators();

    private:
        std::unique_ptr<Fl_Box> indicator_box;
        std::map<Encoder, std::unique_ptr<Fl_Dial>> dials;
        Fl_Color on_color, off_color;
};

class GM2D3StageDiagnostics
{
    public:
        GM2D3StageDiagnostics(int x, int y, int w, int h, const char *label);
        virtual ~GM2D3StageDiagnostics();
        std::unique_ptr<Fl_Box> diagnostics_box;

    private:
        std::unique_ptr<GM2D3StageHistoryPlot> history_plot;
        std::unique_ptr<Fl_Multiline_Output> info;
        std::unique_ptr<GM2D3StageIndicators> indicators;
};

#endif

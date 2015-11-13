#include "gm2d3_stage_diagnostics.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GM2D3StageHistoryPlot::GM2D3StageHistoryPlot(int x, int y, int w, int h) :
    Fl_Chart(x,y,w,h)
{
    type(FL_LINE_CHART);
}

void GM2D3StageHistoryPlot::set_line_color(Fl_Color color) { line_color = color; }

void
GM2D3StageHistoryPlot::plot_random_points(double min, double max, int N)
{
    int i;
    double x;
    char s[32];

    for (i = 0; i < N; i++)
    {
        x = min + rand() / double(RAND_MAX) * (max - min);
        sprintf(s, "%.3f", x);
        add(x, s, line_color);
    }
}

GM2D3StageHistoryPlot::~GM2D3StageHistoryPlot() {}

GM2D3StageInfo::GM2D3StageInfo(int x, int y, int w, int h) :
    Fl_Multiline_Output(x,y,w,h)
{
}

GM2D3StageInfo::~GM2D3StageInfo() {}

GM2D3StageIndicators::GM2D3StageIndicators(int x, int y, int w, int h) :
    Fl_Pack(x,y,w,h)
{
    on_color = FL_GREEN;
    off_color = FL_WHITE;

    type(Fl_Pack::VERTICAL);
    spacing(10);
    align(FL_ALIGN_CENTER);
    {Fl_Pack *top_row = new Fl_Pack(0,0,w, 0.2 * h);
        top_row->type(Fl_Pack::HORIZONTAL);
        top_row->align(FL_ALIGN_CENTER);
        top_row->spacing(0.15 * w);
        a = new Fl_Dial(0.15 * w,0,0.2 * w, 0.2 * w);
        a->type(FL_FILL_DIAL);
        a->color(off_color);
        a->color2(off_color);
        a->align(FL_ALIGN_CENTER);

        b = new Fl_Dial(0.65 * w,0,0.2 * w, 0.2 * w);
        b->type(FL_FILL_DIAL);
        b->color(off_color);
        b->align(FL_ALIGN_CENTER);

        top_row->end();
    }
    {Fl_Pack *bot_row = new Fl_Pack(0,0,w, 0.2 * h);
        bot_row->type(Fl_Pack::HORIZONTAL);
        bot_row->align(FL_ALIGN_CENTER);
        bot_row->spacing(0.15 * w);
        c = new Fl_Dial(0,0,0.2 * w, 0.2 * w);
        c->type(FL_FILL_DIAL);
        c->color(off_color);
        c->align(FL_ALIGN_CENTER);
        d = new Fl_Dial(10,0,0.2 * w, 0.2 * w);
        d->type(FL_FILL_DIAL);
        d->color(off_color);
        d->align(FL_ALIGN_CENTER);
        bot_row->end();
    }
    end();
}

GM2D3StageIndicators::~GM2D3StageIndicators() {}


GM2D3StageDiagnostics::GM2D3StageDiagnostics(int x, int y, int w, int h, const char* label) :
    Fl_Pack(x,y,w,h,label)
{
    type(Fl_Pack::HORIZONTAL);
    spacing(5);
    align(FL_ALIGN_TOP_LEFT);
    history_plot = new GM2D3StageHistoryPlot(0,0,0.6 * w,h);
    info = new GM2D3StageInfo(10,0,0.25*w,h);
    indicators = new GM2D3StageIndicators(20,0,0.15 * w,h);
    end();
}

GM2D3StageDiagnostics::~GM2D3StageDiagnostics() {}


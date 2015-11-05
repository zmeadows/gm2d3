#ifndef GM2D3_STAGE_PLOT
#define GM2D3_STAGE_PLOT

#include <Fl/Fl_Chart.H>
#include <Fl/Fl_Dial.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Multiline_Output.H>

class GM2D3StageDiagnostics : public Fl_Pack
{
    public:
        GM2D3StageDiagnostics(int x, int y, int w, int h, const char* label);
        virtual ~GM2D3StageDiagnostics();

        void set_chart_line_color(Fl_Color color);

    private:
        Fl_Chart *chart;
        GM2D3StageInfo *info;
        GM2D3StageIndicators *indicators;
};

class GM2D3StageIndicators : public Fl_Pack
{
    public:
        GM2D3StageIndicators(int x, int y, int w, int h, const char *label);
        virtual ~GM2D3StageIndicators();

    private:
        Fl_Color on_color, off_color;
}

class GM2D3StageInfo : public Fl_Multiline_Output
{
    public:
        GM2D3StageInfo(int x, int y, int w, int h, const char *label);
        virtual ~GM2D3StageInfo();
}

#endif

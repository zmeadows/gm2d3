#include "gm2d3_options.h"

#include "gm2d3_const.h"


GM2D3ConfigLoader::GM2D3ConfigLoader(int x, int y, int w, int h)
{
    path_display = std::unique_ptr<Fl_Output>(new Fl_Output(x,y,0.8*w,h));
    path_display->label("config file:");
    path_display->align(FL_ALIGN_TOP_LEFT);
    open_button = std::unique_ptr<Fl_Button>(new Fl_Button(x+0.85*w,y,0.15*w,h));
    open_button->label("...");
}

GM2D3ConfigLoader::~GM2D3ConfigLoader(void) {};

GM2D3OptionsGUI::GM2D3OptionsGUI(int x, int y, int w, int h)
{
    options_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,h));
    options_box->box(FL_EMBOSSED_BOX);
    options_box->color(fl_rgb_color(210, 210, 210));
    options_box->align(FL_ALIGN_TOP_LEFT);
    options_box->label("AUTO CONTROL:");
    options_box->labelfont(FL_BOLD);

    enable_history_plot = std::unique_ptr<Fl_Check_Button>
        (new Fl_Check_Button(x+ BOX_EDGE_GAP,y+BOX_EDGE_GAP,w - 2 *BOX_EDGE_GAP,h/6.0));
    enable_history_plot->label("enable history plots");

    enable_indicators = std::unique_ptr<Fl_Check_Button>
        (new Fl_Check_Button(x + BOX_EDGE_GAP,y + BOX_EDGE_GAP + ADJACENT_SPACING + h/6.0,w - 2 *BOX_EDGE_GAP,h/4.0));

    config_loader = std::unique_ptr<GM2D3ConfigLoader>
        (new GM2D3ConfigLoader(x + BOX_EDGE_GAP,y + BOX_EDGE_GAP + 2*ADJACENT_SPACING + h/2.0,w - 2 *BOX_EDGE_GAP,h/4.0));

}

GM2D3OptionsGUI::~GM2D3OptionsGUI() {};
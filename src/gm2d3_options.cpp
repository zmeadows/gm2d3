#include "gm2d3_options.h"

#include "gm2d3_const.h"

void
GM2D3ConfigLoader::static_open_button_pushed(Fl_Widget *_open_button, void *config_loader)
{
    ((GM2D3ConfigLoader *) config_loader)->open_button_pushed(_open_button);
}


void
GM2D3ConfigLoader::open_button_pushed(Fl_Widget *_open_button)
{
    config_file_chooser->show();
}

GM2D3ConfigLoader::GM2D3ConfigLoader(int x, int y, int w, int h)
{
    path_display = std::unique_ptr<Fl_Output>(new Fl_Output(x,y,0.8*w,h));
    path_display->label("CONFIG FILE:");
    path_display->align(FL_ALIGN_TOP_LEFT);

    open_button = std::unique_ptr<Fl_Button>(new Fl_Button(x+0.85*w,y,0.15*w,h - 2));
    open_button->label("...");

    config_file_chooser = std::unique_ptr<Fl_Native_File_Chooser>(new Fl_Native_File_Chooser());

    open_button->callback(static_open_button_pushed, (void *) this);
}

GM2D3ConfigLoader::~GM2D3ConfigLoader(void) {};


GM2D3OptionsGUI::GM2D3OptionsGUI(int x, int y, int w, int h)
{
    options_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,h));
    options_box->box(FL_EMBOSSED_BOX);
    options_box->color(fl_rgb_color(210, 210, 210));
    options_box->align(FL_ALIGN_TOP_LEFT);
    options_box->label("OPTIONS:");
    options_box->labelfont(FL_BOLD);

    const int checkbox_height = int( (h - 5*BOX_EDGE_GAP) / 5.0 );
    const int checkbox_width = int(w - 2*BOX_EDGE_GAP);
    const int x_0 = x + BOX_EDGE_GAP;
    const int y_0 = y + BOX_EDGE_GAP;

    enable_history_plot = std::unique_ptr<Fl_Check_Button>
        (new Fl_Check_Button(x_0, y_0, checkbox_width, checkbox_height));
    enable_history_plot->label("ENABLE HISTORY PLOTS");

    enable_indicators = std::unique_ptr<Fl_Check_Button>
        (new Fl_Check_Button(x_0,y_0 + checkbox_height + BOX_EDGE_GAP, checkbox_width ,checkbox_height));
    enable_indicators->label("ENABLE INDICATORS");

    enable_stats = std::unique_ptr<Fl_Check_Button>
        (new Fl_Check_Button(x_0,y_0 + 2*checkbox_height + 2*BOX_EDGE_GAP, checkbox_width ,checkbox_height));
    enable_stats->label("ENABLE STATS");

    config_loader = std::unique_ptr<GM2D3ConfigLoader>
        (new GM2D3ConfigLoader(x_0,y + 4*checkbox_height + 3*BOX_EDGE_GAP, checkbox_width, checkbox_height));

}

GM2D3OptionsGUI::~GM2D3OptionsGUI() {};

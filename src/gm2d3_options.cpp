#include "gm2d3_options.h"

#include "gm2d3_const.h"

#include <libgen.h>
#include <unistd.h>


GM2D3ConfigLoader::GM2D3ConfigLoader(int x, int y, int w, int h)
{
    path_display = std::unique_ptr<Fl_Output>(new Fl_Output(x,y,0.8*w,h));
    path_display->label("CONFIG FILE:");
    path_display->align(FL_ALIGN_TOP_LEFT);

    open_button = std::unique_ptr<Fl_Button>(new Fl_Button(x+0.85*w,y,0.15*w,h - 2));
    open_button->label("...");

    file_chooser = std::unique_ptr<Fl_Native_File_Chooser>(new Fl_Native_File_Chooser());
}

int
GM2D3ConfigLoader::user_select_config(std::string& path)
{
    switch (file_chooser->show())
    {
    // config file successfully chosen
    case 0:
        char* cpath;
        cpath = strdup(file_chooser->filename());
        path = std::string(cpath, strlen(cpath));
        path_display->value(basename(cpath));
        return 0;

    // user cancelled
    case 1:
        return 1;

    // error
    case -1:
        std::cout << file_chooser->errmsg() << std::endl;
        return -1;

    // who knows
    default:
        return -2;
    }
}

void
GM2D3ConfigLoader::flash_config_path(Fl_Color c)
{
    Fl::lock();

    std::thread flash_thread(&GM2D3ConfigLoader::handle_flash, this, c);

    flash_thread.detach();
    Fl::unlock();
}

void
GM2D3ConfigLoader::handle_flash(Fl_Color c)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        path_display->color(c);
        path_display->redraw();
        Fl::awake();
        usleep(1e5);
        path_display->color(FL_WHITE);
        path_display->redraw();
        Fl::awake();
        usleep(1e5);
    }
}

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

    enable_plot_checkbox = std::unique_ptr<Fl_Check_Button>
                          (new Fl_Check_Button(x_0, y_0, checkbox_width, checkbox_height));
    enable_plot_checkbox->label("ENABLE HISTORY PLOTS");
    enable_plot_checkbox->value(0);

    enable_indicators_checkbox = std::unique_ptr<Fl_Check_Button>
                        (new Fl_Check_Button(x_0,y_0 + checkbox_height + BOX_EDGE_GAP, checkbox_width ,checkbox_height));
    enable_indicators_checkbox->label("ENABLE INDICATORS");
    enable_indicators_checkbox->value(0);

    enable_info_checkbox = std::unique_ptr<Fl_Check_Button>
                  (new Fl_Check_Button(x_0,y_0 + 2*checkbox_height + 2*BOX_EDGE_GAP, checkbox_width ,checkbox_height));
    enable_info_checkbox->label("ENABLE STATS");
    enable_info_checkbox->value(0);

    config_loader = std::unique_ptr<GM2D3ConfigLoader>
                    (new GM2D3ConfigLoader(x_0,y + 4*checkbox_height + 3*BOX_EDGE_GAP, checkbox_width, checkbox_height));

    this->deactivate();
}

void
GM2D3OptionsGUI::deactivate()
{
    Fl::lock();

    enable_plot_checkbox->value(0);
    enable_plot_checkbox->deactivate();

    enable_indicators_checkbox->value(0);
    enable_indicators_checkbox->deactivate();

    enable_info_checkbox->value(0);
    enable_info_checkbox->deactivate();

    Fl::awake();
    Fl::unlock();
}

void
GM2D3OptionsGUI::activate()
{
    Fl::lock();

    enable_plot_checkbox->activate();
    enable_indicators_checkbox->activate();
    enable_info_checkbox->activate();

    Fl::awake();
    Fl::unlock();
}

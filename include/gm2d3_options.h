#pragma once

#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Output.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl.H>

#include <memory>
#include <string>
#include <iostream>
#include <thread>

class GM2D3ConfigLoader
{
public:
    GM2D3ConfigLoader(int x, int y, int w, int h);

    int user_select_config(std::string &path);
    void set_path_display_color(Fl_Color c) { path_display->color(c); }
    void flash_config_path(Fl_Color c);

    std::unique_ptr<Fl_Output> path_display;
    std::unique_ptr<Fl_Button> open_button;

private:
    void handle_flash(Fl_Color c);
    std::unique_ptr<Fl_Native_File_Chooser> file_chooser;
};

class GM2D3OptionsGUI
{
public:
    GM2D3OptionsGUI(int x, int y, int w, int h);

    void activate();
    void deactivate();

    std::unique_ptr<GM2D3ConfigLoader> config_loader;
    std::unique_ptr<Fl_Check_Button> enable_plot_checkbox;
    std::unique_ptr<Fl_Check_Button> enable_indicators_checkbox;
    std::unique_ptr<Fl_Check_Button> enable_info_checkbox;

private:
    std::unique_ptr<Fl_Box> options_box;
};


#pragma once

#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Output.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <memory>
#include <iostream>

class GM2D3ConfigLoader {
    public:
        GM2D3ConfigLoader(int x, int y, int w, int h);
        virtual ~GM2D3ConfigLoader(void);

    private:
        std::unique_ptr<Fl_Output> path_display;
        std::unique_ptr<Fl_Button> open_button;
        std::unique_ptr<Fl_Native_File_Chooser> config_file_chooser;

        static void static_open_button_pushed(Fl_Widget *_open_button, void *config_loader);
        void open_button_pushed(Fl_Widget *_open_button);
};

class GM2D3OptionsGUI {
    public:
        GM2D3OptionsGUI(int x, int y, int w, int h);
        virtual ~GM2D3OptionsGUI(void);
    private:
        std::unique_ptr<Fl_Box> options_box;
        std::unique_ptr<Fl_Check_Button> enable_history_plot;
        std::unique_ptr<Fl_Check_Button> enable_indicators;
        std::unique_ptr<Fl_Check_Button> enable_stats;
        std::unique_ptr<GM2D3ConfigLoader> config_loader;
};


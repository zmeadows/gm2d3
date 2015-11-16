#pragma once

#include <Fl/Fl_Check_Button.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Output.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <memory>
#include <string>
#include <iostream>

class GM2D3ConfigLoader {
    public:
        GM2D3ConfigLoader(int x, int y, int w, int h);
        virtual ~GM2D3ConfigLoader(void);

        int get_config_path(std::string &path);

        void callback(Fl_Callback_p cb, void *p) { open_button->callback(cb,p); }

    private:
        std::unique_ptr<Fl_Native_File_Chooser> file_chooser;
        std::unique_ptr<Fl_Output> path_display;
        std::unique_ptr<Fl_Button> open_button;
};

class GM2D3OptionsGUI {
    public:
        GM2D3OptionsGUI(int x, int y, int w, int h);
        virtual ~GM2D3OptionsGUI(void);

        std::unique_ptr<GM2D3ConfigLoader> config_loader;

    private:
        std::unique_ptr<Fl_Box> options_box;
        std::unique_ptr<Fl_Check_Button> enable_history_plot;
        std::unique_ptr<Fl_Check_Button> enable_indicators;
        std::unique_ptr<Fl_Check_Button> enable_stats;
};

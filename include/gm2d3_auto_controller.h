#pragma once

#include "gm2d3_stage_controller.h"

#include <Fl/Fl_Input.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Menu_Button.H>

#include <map>

class GM2D3AutoControlGUI {
    public:
        GM2D3AutoControlGUI(int x, int y, int w, int h);
        virtual ~GM2D3AutoControlGUI();

        void set_input_text(Axis axis, std::string str);
        void set_input_editable(Axis axis, bool readonly);

    private:
        bool enabled;

        std::unique_ptr<Fl_Box> auto_control_box;

        std::unique_ptr<Fl_Pack> horizontal_pack;
        std::unique_ptr<Fl_Pack> input_box_pack;
        std::unique_ptr<Fl_Pack> auto_button_pack;

        std::map<Axis, std::unique_ptr<Fl_Input>> user_position_inputs;

        std::unique_ptr<Fl_Menu_Button> preset_positions_menu;

        std::unique_ptr<Fl_Button> calibrate_button;
        std::unique_ptr<Fl_Button> go_button;
        std::unique_ptr<Fl_Button> kill_button;
};

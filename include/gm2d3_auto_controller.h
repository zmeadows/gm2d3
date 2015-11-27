#pragma once

#include "gm2d3_const.h"
#include "gm2d3_util.h"
#include "gm2d3_fltk_util.h"

#include <Fl/Fl_Input.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Button.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Menu_Button.H>
#include <Fl/fl_draw.H>

#include <map>
#include <memory>

class GM2D3AutoControlInput : public Fl_Input {
    public:
        GM2D3AutoControlInput(int x, int y, int w, int h, Axis _axis);

        const Axis axis;
};

class GM2D3AutoControlGUI {
    public:
        GM2D3AutoControlGUI(int x, int y, int w, int h);

        void disable_input(Axis axis);
        void enable_input(Axis axis);

        void activate();
        void deactivate();

        void set_kill_button_callback(Fl_Callback *cb, void *_gm2d3) { kill_button->callback(cb, _gm2d3); }

    private:
        std::unique_ptr<Fl_Box> auto_control_box;

        std::unique_ptr<Fl_Pack> horizontal_pack;
        std::unique_ptr<Fl_Pack> input_box_pack;
        std::unique_ptr<Fl_Pack> auto_button_pack;

        std::unique_ptr<Fl_Button> calibrate_button;
        std::unique_ptr<Fl_Button> go_button;
        std::unique_ptr<Fl_Button> kill_button;

        std::map<Axis, std::unique_ptr<GM2D3AutoControlInput>> user_position_inputs;

        std::unique_ptr<Fl_Menu_Button> preset_positions_menu;
};

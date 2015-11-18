#pragma once

#include <Fl/Enumerations.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Light_Button.H>

#include <map>

#include "gm2d3_const.h"

class GM2D3ManualControlGUI {
    public:
        GM2D3ManualControlGUI(int x, int y, int w, int h);
        virtual ~GM2D3ManualControlGUI();

        void callback(Axis axis, MotorState m, Fl_Callback_p cb, void *p) {
            buttons[axis][m]->callback(cb,p);
        }

    private:
        std::unique_ptr<Fl_Box> manual_control_box;

        std::unique_ptr<Fl_Pack> vertical_pack;
        std::unique_ptr<Fl_Pack> azimuthal_button_pack;
        std::unique_ptr<Fl_Pack> vertical_button_pack;
        std::unique_ptr<Fl_Pack> radial_button_pack;

        std::map<Axis, std::map<MotorState, std::unique_ptr<Fl_Button>>> buttons;
};

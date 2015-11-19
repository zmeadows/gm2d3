#pragma once

#include <Fl/Enumerations.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Light_Button.H>

#include <map>
#include <memory>

#include "gm2d3_const.h"

class GM2D3ManualControlButton : public Fl_Button {
    public:
        GM2D3ManualControlButton(int x, int y, int w, int h, Axis _axis, MotorState _motor_state);
        virtual ~GM2D3ManualControlButton();

        const Axis axis;
        const MotorState motor_state;
};



class GM2D3ManualControlGUI {
    public:
        GM2D3ManualControlGUI(int x, int y, int w, int h);
        virtual ~GM2D3ManualControlGUI();

        void callback(Fl_Callback_p cb, void *p) {
            for (auto& a : ALL_AXES) {
                for (auto& m : ALL_MOTOR_STATES) {
                    buttons[a][m]->callback(cb,p);
                }
            }
        }

        void enable_axis(Axis axis);
        void disable_axis(Axis axis);

    private:
        std::unique_ptr<Fl_Box> manual_control_box;

        std::unique_ptr<Fl_Pack> vertical_pack;
        std::unique_ptr<Fl_Pack> azimuthal_button_pack;
        std::unique_ptr<Fl_Pack> vertical_button_pack;
        std::unique_ptr<Fl_Pack> radial_button_pack;

        std::map<Axis, std::map<MotorState, std::unique_ptr<GM2D3ManualControlButton>>> buttons;
};



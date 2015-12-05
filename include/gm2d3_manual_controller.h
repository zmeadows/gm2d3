#pragma once

#include <Fl/Enumerations.H>
#include <Fl/Fl_Pack.H>
#include <Fl/Fl_Box.H>
#include <Fl/Fl_Light_Button.H>
#include <Fl/Fl.H>

#include <map>
#include <set>
#include <memory>

#include "gm2d3_const.h"
#include "gm2d3_fltk_util.h"

class GM2D3ManualControlButton : public Fl_Button
{
public:
    GM2D3ManualControlButton(int x, int y, int w, int h, Axis _axis, MotorState _motor_state);

    const Axis axis;
    const MotorState motor_state;
};



class GM2D3ManualControlGUI
{
public:
    GM2D3ManualControlGUI(int x, int y, int w, int h);

    std::map<Axis, std::map<MotorState, std::unique_ptr<GM2D3ManualControlButton>>> buttons;

    void activate(std::set<Axis> axes);
    void deactivate(void);

private:
    void enable_axis(Axis axis);
    void disable_axis(Axis axis);

    std::unique_ptr<Fl_Box> manual_control_box;
    std::unique_ptr<Fl_Pack> vertical_pack;
    std::unique_ptr<Fl_Pack> azimuthal_button_pack;
    std::unique_ptr<Fl_Pack> vertical_button_pack;
    std::unique_ptr<Fl_Pack> radial_button_pack;

};



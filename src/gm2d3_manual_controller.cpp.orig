#include "gm2d3_manual_controller.h"
#include "gm2d3_util.h"

GM2D3ManualControlButton::GM2D3ManualControlButton(int x, int y, int w, int h,
        Axis _axis, MotorState _motor_state)
    : Fl_Button(x,y,w,h), axis(_axis), motor_state(_motor_state)
{
    type(FL_RADIO_BUTTON);
    down_color(YELLOW());

    switch (motor_state)
    {
        case MotorState::CW:
            label("CW");
            break;
        case MotorState::CCW:
            label("CCW");
            break;
        case MotorState::OFF:
            label("OFF");
            break;
    }
}

GM2D3ManualControlGUI::GM2D3ManualControlGUI(int x, int y, int w, int h)
{
    // calculate size/spacing properties
    const int button_label_gap = 90;
    const int button_height = int( (h - 2*ADJACENT_SPACING - 2*BOX_EDGE_GAP) / 3.0 );
    const int button_width = int( (w - button_label_gap - 2*ADJACENT_SPACING - 2*BOX_EDGE_GAP ) / 3.0 );

    manual_control_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,h));
    manual_control_box->box(FL_EMBOSSED_BOX);
    manual_control_box->color(fl_rgb_color(210,210,210));
    manual_control_box->align(FL_ALIGN_TOP_LEFT);
    manual_control_box->label("MANUAL CONTROL:");
    manual_control_box->labelfont(FL_BOLD);

    {vertical_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(x + button_label_gap + BOX_EDGE_GAP,
                y + BOX_EDGE_GAP, w, h));
    vertical_pack->type(Fl_Pack::VERTICAL);
    vertical_pack->labelfont(FL_BOLD);
    vertical_pack->align(FL_ALIGN_TOP_LEFT);
    vertical_pack->spacing(ADJACENT_SPACING);

    {azimuthal_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0, 0,
                2*button_width + ADJACENT_SPACING, button_height));
    azimuthal_button_pack->type(Fl_Pack::HORIZONTAL);
    azimuthal_button_pack->spacing(ADJACENT_SPACING);
    azimuthal_button_pack->align(FL_ALIGN_LEFT);
    azimuthal_button_pack->label("AZIMUTHAL: ");

    buttons[Axis::AZIMUTHAL][MotorState::CW] =
        std::unique_ptr<GM2D3ManualControlButton> (new GM2D3ManualControlButton
                (0, 0, button_width, button_height, Axis::AZIMUTHAL, MotorState::CW));

    buttons[Axis::AZIMUTHAL][MotorState::CCW] =
        std::unique_ptr<GM2D3ManualControlButton>(new GM2D3ManualControlButton
                (button_width + ADJACENT_SPACING, 0, button_width, button_height,
                 Axis::AZIMUTHAL, MotorState::CCW));

    buttons[Axis::AZIMUTHAL][MotorState::OFF] =
        std::unique_ptr<GM2D3ManualControlButton>(new GM2D3ManualControlButton(
                2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height,
                Axis::AZIMUTHAL, MotorState::OFF));

    azimuthal_button_pack->end();
    }

    {vertical_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0,
                button_height + ADJACENT_SPACING, 2*button_width + ADJACENT_SPACING, button_height));

    vertical_button_pack->type(Fl_Pack::HORIZONTAL);
    vertical_button_pack->spacing(ADJACENT_SPACING);
    vertical_button_pack->align(FL_ALIGN_LEFT);
    vertical_button_pack->label("VERTICAL: ");

    buttons[Axis::VERTICAL][MotorState::CW] =
        std::unique_ptr<GM2D3ManualControlButton> (new GM2D3ManualControlButton
                (0, 0, button_width, button_height, Axis::VERTICAL, MotorState::CW));

    buttons[Axis::VERTICAL][MotorState::CCW] =
        std::unique_ptr<GM2D3ManualControlButton> (new GM2D3ManualControlButton
                (button_width + ADJACENT_SPACING, 0, button_width, button_height,
                 Axis::VERTICAL, MotorState::CCW));

    buttons[Axis::VERTICAL][MotorState::OFF] =
        std::unique_ptr<GM2D3ManualControlButton> (new GM2D3ManualControlButton
                (2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height,
                 Axis::VERTICAL, MotorState::OFF));

    vertical_button_pack->end();
    }

    {radial_button_pack = std::unique_ptr<Fl_Pack>
        (new Fl_Pack(0, 2*button_height + 2*ADJACENT_SPACING,
                     2*button_width + ADJACENT_SPACING, button_height));
    radial_button_pack->type(Fl_Pack::HORIZONTAL);
    radial_button_pack->spacing(ADJACENT_SPACING);
    radial_button_pack->align(FL_ALIGN_LEFT);
    radial_button_pack->label("RADIAL: ");

    buttons[Axis::RADIAL][MotorState::CW] =
        std::unique_ptr<GM2D3ManualControlButton>(new GM2D3ManualControlButton
                (0, 0, button_width, button_height, Axis::RADIAL, MotorState::CW));

    buttons[Axis::RADIAL][MotorState::CCW] =
        std::unique_ptr<GM2D3ManualControlButton>(new GM2D3ManualControlButton
                (button_width + ADJACENT_SPACING, 0, button_width, button_height,
                 Axis::RADIAL, MotorState::CCW));

    buttons[Axis::RADIAL][MotorState::OFF] =
        std::unique_ptr<GM2D3ManualControlButton>(new GM2D3ManualControlButton
                (2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height,
                 Axis::RADIAL, MotorState::OFF));

    radial_button_pack->end();
    }

    vertical_pack->end();
    }

    deactivate();
}

void
GM2D3ManualControlGUI::enable_axis(Axis axis) 
{
    Fl::lock();

    for (auto& m : ALL_MOTOR_STATES)
    {
        buttons[axis][m]->activate();
    }

    buttons[axis][MotorState::OFF]->setonly();
    Fl::awake();

    Fl::unlock();
}

void
GM2D3ManualControlGUI::disable_axis(Axis axis) 
{
    Fl::lock();

    buttons[axis][MotorState::OFF]->setonly();

    for (auto& m : ALL_MOTOR_STATES)
    {
        buttons[axis][m]->deactivate();
    }
    Fl::awake();

    Fl::unlock();
}

void
GM2D3ManualControlGUI::activate()
{
    for (auto &a : ALL_AXES) { enable_axis(a); }
}

void
GM2D3ManualControlGUI::deactivate()
{
    for (auto &a : ALL_AXES) { disable_axis(a); }
}

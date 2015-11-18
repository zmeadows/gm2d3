#include "gm2d3_manual_controller.h"

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
        std::unique_ptr<Fl_Button>(new Fl_Button(0, 0, button_width, button_height, "CW"));
    buttons[Axis::AZIMUTHAL][MotorState::CW]->type(FL_RADIO_BUTTON);
    buttons[Axis::AZIMUTHAL][MotorState::CW]->down_color(YELLOW());

    buttons[Axis::AZIMUTHAL][MotorState::CCW] = std::unique_ptr<Fl_Button>(new Fl_Button(
                button_width + ADJACENT_SPACING, 0, button_width, button_height, "CCW"));
    buttons[Axis::AZIMUTHAL][MotorState::CCW]->type(FL_RADIO_BUTTON);
    buttons[Axis::AZIMUTHAL][MotorState::CCW]->down_color(YELLOW());

    buttons[Axis::AZIMUTHAL][MotorState::OFF] = std::unique_ptr<Fl_Button>(new Fl_Button(
                2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height, "OFF"));
    buttons[Axis::AZIMUTHAL][MotorState::OFF]->type(FL_RADIO_BUTTON);
    buttons[Axis::AZIMUTHAL][MotorState::OFF]->down_color(YELLOW());
    buttons[Axis::AZIMUTHAL][MotorState::OFF]->value(1);

    azimuthal_button_pack->end();
    }

    {vertical_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0,
                button_height + ADJACENT_SPACING, 2*button_width + ADJACENT_SPACING, button_height));

    vertical_button_pack->type(Fl_Pack::HORIZONTAL);
    vertical_button_pack->spacing(ADJACENT_SPACING);
    vertical_button_pack->align(FL_ALIGN_LEFT);
    vertical_button_pack->label("VERTICAL: ");

    buttons[Axis::VERTICAL][MotorState::CW] = std::unique_ptr<Fl_Button>
        (new Fl_Button(0, 0, button_width, button_height, "CW"));
    buttons[Axis::VERTICAL][MotorState::CW]->type(FL_RADIO_BUTTON);
    buttons[Axis::VERTICAL][MotorState::CW]->down_color(YELLOW());

    buttons[Axis::VERTICAL][MotorState::CCW] = std::unique_ptr<Fl_Button>
        (new Fl_Button( button_width + ADJACENT_SPACING, 0, button_width, button_height, "CCW"));
    buttons[Axis::VERTICAL][MotorState::CCW]->type(FL_RADIO_BUTTON);
    buttons[Axis::VERTICAL][MotorState::CCW]->down_color(YELLOW());

    buttons[Axis::VERTICAL][MotorState::OFF] = std::unique_ptr<Fl_Button>
        (new Fl_Button( 2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height, "OFF"));
    buttons[Axis::VERTICAL][MotorState::OFF]->type(FL_RADIO_BUTTON);
    buttons[Axis::VERTICAL][MotorState::OFF]->down_color(YELLOW());
    buttons[Axis::VERTICAL][MotorState::OFF]->value(1);

    vertical_button_pack->end();
    }

    {radial_button_pack = std::unique_ptr<Fl_Pack>
        (new Fl_Pack(0, 2*button_height + 2*ADJACENT_SPACING,
                     2*button_width + ADJACENT_SPACING, button_height));
    radial_button_pack->type(Fl_Pack::HORIZONTAL);
    radial_button_pack->spacing(ADJACENT_SPACING);
    radial_button_pack->align(FL_ALIGN_LEFT);
    radial_button_pack->label("RADIAL: ");

    buttons[Axis::RADIAL][MotorState::CW] = std::unique_ptr<Fl_Button>(new Fl_Button(0, 0,
                button_width, button_height, "CW"));
    buttons[Axis::RADIAL][MotorState::CW]->type(FL_RADIO_BUTTON);
    buttons[Axis::RADIAL][MotorState::CW]->down_color(YELLOW());

    buttons[Axis::RADIAL][MotorState::CCW] = std::unique_ptr<Fl_Button>(new Fl_Button(
                button_width + ADJACENT_SPACING, 0, button_width, button_height,"CCW"));
    buttons[Axis::RADIAL][MotorState::CCW]->type(FL_RADIO_BUTTON);
    buttons[Axis::RADIAL][MotorState::CCW]->down_color(YELLOW());

    buttons[Axis::RADIAL][MotorState::OFF] = std::unique_ptr<Fl_Button>(new Fl_Button(
                2*button_width + 2*ADJACENT_SPACING, 0, button_width, button_height,"OFF"));
    buttons[Axis::RADIAL][MotorState::OFF]->type(FL_RADIO_BUTTON);
    buttons[Axis::RADIAL][MotorState::OFF]->down_color(YELLOW());
    buttons[Axis::RADIAL][MotorState::OFF]->value(1);

    radial_button_pack->end();
    }

    vertical_pack->end();
    }

}

GM2D3ManualControlGUI::~GM2D3ManualControlGUI() {}

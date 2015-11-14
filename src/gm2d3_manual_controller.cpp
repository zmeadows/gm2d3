#include "gm2d3_manual_controller.h"

GM2D3ManualControlGUI::GM2D3ManualControlGUI(int x, int y, int w, int h)
{
    // calculate size/spacing properties
    const int button_label_gap = 90;
    const int button_height = int( (h - 2*ADJACENT_SPACING - 2*BOX_EDGE_GAP) / 3.0 );
    const int button_width = int( (w - button_label_gap - 1*ADJACENT_SPACING - 2*BOX_EDGE_GAP ) / 2.0 );

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

    cw_buttons[Axis::AZIMUTHAL] = std::unique_ptr<Fl_Button>(new Fl_Button(0, 0,
                button_width, button_height, "CW"));
    cw_buttons[Axis::AZIMUTHAL]->type(FL_TOGGLE_BUTTON);
    cw_buttons[Axis::AZIMUTHAL]->down_color(DARK_RED());

    ccw_buttons[Axis::AZIMUTHAL] = std::unique_ptr<Fl_Button>(new Fl_Button(
                button_width + ADJACENT_SPACING, 0, button_width, button_height, "CCW"));
    ccw_buttons[Axis::AZIMUTHAL]->type(FL_TOGGLE_BUTTON);
    ccw_buttons[Axis::AZIMUTHAL]->down_color(DARK_RED());

    azimuthal_button_pack->end();
    }

    {vertical_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0,
                button_height + ADJACENT_SPACING, 2*button_width + ADJACENT_SPACING, button_height));

    vertical_button_pack->type(Fl_Pack::HORIZONTAL);
    vertical_button_pack->spacing(ADJACENT_SPACING);
    vertical_button_pack->align(FL_ALIGN_LEFT);
    vertical_button_pack->label("VERTICAL: ");

    cw_buttons[Axis::VERTICAL] = std::unique_ptr<Fl_Button>(new Fl_Button(0, 0,
                button_width, button_height, "CW"));
    cw_buttons[Axis::VERTICAL]->type(FL_TOGGLE_BUTTON);
    cw_buttons[Axis::VERTICAL]->down_color(DARK_GREEN());

    ccw_buttons[Axis::VERTICAL] = std::unique_ptr<Fl_Button>(new Fl_Button(
                button_width + ADJACENT_SPACING, 0, button_width, button_height, "CCW"));
    ccw_buttons[Axis::VERTICAL]->type(FL_TOGGLE_BUTTON);
    ccw_buttons[Axis::VERTICAL]->down_color(DARK_GREEN());

    vertical_button_pack->end();
    }

    {radial_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0,
                2*button_height + 2*ADJACENT_SPACING,
                2*button_width + ADJACENT_SPACING, button_height));
    radial_button_pack->type(Fl_Pack::HORIZONTAL);
    radial_button_pack->spacing(ADJACENT_SPACING);
    radial_button_pack->align(FL_ALIGN_LEFT);
    radial_button_pack->label("RADIAL: ");

    cw_buttons[Axis::RADIAL] = std::unique_ptr<Fl_Button>(new Fl_Button(0, 0,
                button_width, button_height, "CW"));
    cw_buttons[Axis::RADIAL]->type(FL_TOGGLE_BUTTON);
    cw_buttons[Axis::RADIAL]->down_color(DARK_BLUE());

    ccw_buttons[Axis::RADIAL] = std::unique_ptr<Fl_Button>(new Fl_Button(
                button_width + ADJACENT_SPACING, 0, button_width, button_height,"CCW"));
    ccw_buttons[Axis::RADIAL]->type(FL_TOGGLE_BUTTON);
    ccw_buttons[Axis::RADIAL]->down_color(DARK_BLUE());

    radial_button_pack->end();
    }

    vertical_pack->end();
    }

}

GM2D3ManualControlGUI::~GM2D3ManualControlGUI() {}

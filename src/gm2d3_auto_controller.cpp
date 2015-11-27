#include "gm2d3_auto_controller.h"

#include <map>


GM2D3AutoControlInput::GM2D3AutoControlInput(int x, int y, int w, int h, Axis _axis)
    : Fl_Input(x,y,w,h), axis(_axis)
{
    align(FL_ALIGN_LEFT);

    switch (axis)
    {
        case Axis::AZIMUTHAL:
            label("AZIMUTHAL: ");
            break;
        case Axis::VERTICAL:
            label("VERTICAL: ");
            break;
        case Axis::RADIAL:
            label("RADIAL: ");
            break;
    }
}

GM2D3AutoControlInput::~GM2D3AutoControlInput() {}

GM2D3AutoControlGUI::GM2D3AutoControlGUI(int x, int y, int w, int h)
{
    const int input_label_gap = 90;
    const int input_height = int( (h - 5*BOX_EDGE_GAP) / 4.0 );
    const int input_width = int( (w - input_label_gap - 2*BOX_EDGE_GAP - ADJACENT_SPACING) / 2.0);
    const int button_height = int( (h - 2*ADJACENT_SPACING - 2*BOX_EDGE_GAP) / 3.0 );

    auto_control_box = std::unique_ptr<Fl_Box>(new Fl_Box(x,y,w,h));
    auto_control_box->box(FL_EMBOSSED_BOX);
    auto_control_box->color(fl_rgb_color(210, 210, 210));
    auto_control_box->align(FL_ALIGN_TOP_LEFT);
    auto_control_box->label("AUTO CONTROL:");
    auto_control_box->labelfont(FL_BOLD);

    {horizontal_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack
            (x + input_label_gap + BOX_EDGE_GAP, y + BOX_EDGE_GAP, w, h));
        horizontal_pack->type(Fl_Pack::HORIZONTAL);
        horizontal_pack->spacing(ADJACENT_SPACING);

        {input_box_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(0,0,input_width,h));
            input_box_pack->type(Fl_Pack::VERTICAL);
            input_box_pack->spacing(BOX_EDGE_GAP);

            preset_positions_menu = std::unique_ptr<Fl_Menu_Button>(new Fl_Menu_Button(0,30,input_width,input_height, "PRESETS: "));
            preset_positions_menu->align(FL_ALIGN_LEFT);

            user_position_inputs[Axis::AZIMUTHAL] = std::unique_ptr<GM2D3AutoControlInput>
                (new GM2D3AutoControlInput(0,0,input_width,input_height, Axis::AZIMUTHAL));
            user_position_inputs[Axis::VERTICAL]  = std::unique_ptr<GM2D3AutoControlInput>
                (new GM2D3AutoControlInput(0,10,input_width,input_height, Axis::VERTICAL));
            user_position_inputs[Axis::RADIAL]    = std::unique_ptr<GM2D3AutoControlInput>
                (new GM2D3AutoControlInput(0,20,input_width,input_height, Axis::RADIAL));

            input_box_pack->end();
        }

        {auto_button_pack = std::unique_ptr<Fl_Pack>(new Fl_Pack(10,0,input_width,h));
            auto_button_pack->type(Fl_Pack::VERTICAL);
            auto_button_pack->spacing(ADJACENT_SPACING);

            calibrate_button = std::unique_ptr<Fl_Button>(new Fl_Button(0,0,
                        input_width, button_height, "CALIBRATE"));
            calibrate_button->align(FL_ALIGN_CENTER);
            calibrate_button->type(FL_TOGGLE_BUTTON);
            calibrate_button->color2(YELLOW());

            go_button = std::unique_ptr<Fl_Button>(new Fl_Button(0,10,
                        input_width,button_height, "GO"));
            go_button->align(FL_ALIGN_CENTER);
            go_button->type(FL_TOGGLE_BUTTON);
            go_button->color2(LIGHT_GREEN());

            kill_button = std::unique_ptr<Fl_Button>(new Fl_Button(0,20,
                        input_width,button_height,"KILL"));
            kill_button->color(FL_RED);
            kill_button->color2(FL_RED);

            auto_button_pack->end();
        }

        horizontal_pack->end();
    }

    for (auto& a : ALL_AXES) { disable_input(a); }
    deactivate();
}

GM2D3AutoControlGUI::~GM2D3AutoControlGUI() {}

void
GM2D3AutoControlGUI::enable_input(Axis axis)
{
    user_position_inputs[axis]->activate();
    user_position_inputs[axis]->textcolor(FL_BLACK);
    user_position_inputs[axis]->textfont(0);
    user_position_inputs[axis]->value("");
    user_position_inputs[axis]->readonly(0);
}

void
GM2D3AutoControlGUI::disable_input(Axis axis)
{
    user_position_inputs[axis]->deactivate();
    user_position_inputs[axis]->textcolor(FL_RED);
    user_position_inputs[axis]->textfont(FL_BOLD);
    user_position_inputs[axis]->value("DISCONNECTED");
    user_position_inputs[axis]->readonly(1);
}

void
GM2D3AutoControlGUI::activate()
{
    calibrate_button->activate();
    go_button->activate();
}

void
GM2D3AutoControlGUI::deactivate()
{
    calibrate_button->deactivate();
    go_button->deactivate();
}

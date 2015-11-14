#pragma once

#include <Fl/Fl_Draw.H>

enum class Axis {
    AZIMUTHAL,
    VERTICAL,
    RADIAL
};

enum class MotorDirection {
    CW,
    CCW
};

// ENCODER
const int ABS_CODE_BITS = 10;

// GUI
const int WINDOW_EDGE_GAP = 20;
const int ADJACENT_SPACING = 5;
const int BOX_EDGE_GAP = 7;

Fl_Color LIGHT_RED();
Fl_Color DARK_RED();

Fl_Color LIGHT_GREEN();
Fl_Color DARK_GREEN();

Fl_Color LIGHT_BLUE();
Fl_Color DARK_BLUE();

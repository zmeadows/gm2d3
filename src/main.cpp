#include <Fl/Fl.H>
#include "gm2d3_window.h"

int
main(int argc, char** args)
{
    GM2D3Window gm2d3Window(800,800,"Test");
    gm2d3Window.show();

    Fl::run();
    gm2d3Window.end();

    return 0;
}

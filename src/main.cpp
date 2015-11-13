#include <Fl/Fl.H>
#include "gm2d3_window.h"

#include <stdlib.h>
#include <time.h>

int
main(int argc, char** args)
{
    // seed the RNG
    srand(time(NULL));

    GM2D3Window gm2d3Window(800,800,"GM2D3");
    gm2d3Window.show();

    Fl::run();
    gm2d3Window.end();

    return 0;
}

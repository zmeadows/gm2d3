#include <Fl/Fl.H>
#include "gm2d3.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <memory>

int
main(int argc, char** args)
{
    // seed the RNG
    srand(time(NULL));

    std::unique_ptr<GM2D3> gm2d3(new GM2D3(1100,665));

    Fl::run();

    return 0;
}

#include <Fl/Fl.H>
#include "gm2d3.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

int
main(int argc, char** args)
{
    // seed the RNG
    srand(time(NULL));

    GM2D3 *gm2d3 = new GM2D3(1000,660);

    std::cout << gm2d3 << "\n";
    Fl::run();

    return 0;
}

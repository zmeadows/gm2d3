#include <Fl/Fl.H>
#include "gm2d3.h"

#include "gm2d3_util.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <memory>

int
main(int argc, char** argv)
{
    /* seed the RNG */
    srand(time(NULL));

#ifdef GM2D3_USE_RPI
    std::cout << "using rpi" << std::endl;
#endif

    /* parse command-line arguments */
    int c;
    while ((c = getopt (argc, argv, "V:")) != -1)
    {
        switch (c)
        {
            case 'V':
                gDEBUG_LEVEL = atoi(optarg);
                std::cout << gDEBUG_LEVEL << std::endl;
                break;
            default:
                abort();
        }
    }

    std::unique_ptr<GM2D3> gm2d3(new GM2D3(1100,665));

    // set up multi-threading and run FLTK
    Fl::lock();
    Fl::run();

    // process all multi-threaded events
    while (Fl::wait() > 0) {}

    return 0;
}

#include "gm2d3.h"

#include "gm2d3_util.h"
#include "gm2d3_const.h"
#include "../cmake/gm2d3_cmake.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <memory>

#include <Fl/Fl.H>

#ifdef GM2D3_USE_RPI
#include <pigpio.h>
#endif

void const print_info(void);

int
main(int argc, char** argv)
{
    /* seed the RNG */
    srand(time(NULL));

#ifdef GM2D3_USE_RPI
    // TODO: out more useful message (and on error)
    if (gpioInitialise() < 0) return 1;
    std::cout << "using rpi" << std::endl;
#endif

    print_logo();
    print_info();

    /* parse command-line arguments */
    int c;
    while ((c = getopt (argc, argv, "V:")) != -1)
    {
        switch (c)
        {
            case 'V':
                gDEBUG_LEVEL = atoi(optarg);
                break;
            default:
                abort();
        }
    }

    std::shared_ptr<GM2D3> gm2d3(new GM2D3(1100,665));

    // set up multi-threading and run FLTK
    Fl::lock();
    Fl::run();

    // process all multi-threaded events
    while (Fl::wait() > 0) {}

    return 0;
}

void const
print_info()
{
    std::cout
        << "VERSION " << GM2D3_VERSION_MAJOR << "." << GM2D3_VERSION_MINOR << std::endl
        << "COMPILED ON " << __DATE__ << " AT " << __TIME__ << std::endl << std::endl;
}

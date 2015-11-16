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

    Fl::run();

    return 0;
}

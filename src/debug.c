#include "debug.h"

typedef struct _Error Error;
struct _Error {
    int   errno;
    char *details;
};

void
_perror ( char *function )
{
    Error error[] = {
        { _E_OK,   "Success."                             },
        { _E_FAIL, "Failure, no given details."           },
        { _E_SAME, "Starting point equals ending point."  },
        { _E_OBS,  "Starting point is an obstacle."       },
        { _E_OUT,  "Starting point is out of map bounds." },
        { _E_OBS_, "Ending point is an obstacle."         },
        { _E_OUT_, "Ending point is out of map bounds."   },
        { _E_LOOP, "Max loop number reached."             },
        { _E_NOP,  "No path exists."                      }
    };

    printf ( "\033[1;31m%s: %s\033[0m\n", function, error[errno].details );
}

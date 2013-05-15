#include "debug.h"
#include "errno.h"
#include <stdio.h>

struct errmsg {
  int   errno;
  char *details;
};

void
_perror (char *function)
{
  extern int errno;
  struct errmsg errmsg[] = {
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
  
  printf ("%s: %s\n", function, errmsg[errno].details);
}

#include "nextstep.h"

int step = 0;

int
nextstep ( int *x, int *y )
{
    if ( step >= pathlength_ )
    {
        step = 0;
        return _E_FAIL;
    }

    *x = path_[step].x;
    *y = path_[step].y;
    step++;

    return _E_OK;
}

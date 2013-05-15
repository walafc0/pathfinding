#include "nextstep.h"
#include "findpath.h"
#include "errno.h"

int
nextstep (int *x, int *y)
{
  static int step = 0;

  if (step >= pathlength_)
    {
      step  = 0;

      errno = _E_FAIL;
      return -1;
    }
  
  *x = path_[step].x;
  *y = path_[step].y;
  step++;
  
  errno = _E_OK;
  return 0;
}

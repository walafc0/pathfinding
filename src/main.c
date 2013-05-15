#define _XOPEN_SOURCE 700

#ifdef LOCAL
#include "local.h"
#endif

#ifdef DEBUG
#include "debug.h"
#endif

#ifdef DISPLAY
#include "display.h"
#endif

#include "findpath.h"
#include "nextstep.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  int err;

  #ifdef DISPLAY
  pthread_mutex_lock (&mutex);
  pthread_create (&tid, NULL, display, NULL);
  pthread_cond_wait (&cond, &mutex);
  pthread_mutex_unlock (&mutex);

  obstacles ();
  #endif

  // Premier déplacement :
  x = getX ();
  y = getY ();

  #ifdef DEBUG
  printf ("Start\tx = %d\ty = %d\n", x, y);
  #endif

  #ifdef DISPLAY
  startx = x;
  starty = y;
  #endif

  err = findpath (x, y, 13, 37);
  if (err == -1)
    {
      #ifdef DEBUG
      _perror ("findpath");
      #endif

      exit (EXIT_FAILURE);
    }

  while (!(err = nextstep (&x, &y)))
    {
      #ifdef DEBUG
      printf ("Next \tx = %d\ty = %d\n", x, y);
      #endif
    }

  #ifdef DISPLAY
  paths ();
  #endif

  // Second déplacement :
  x = getX ();
  y = getY ();

  #ifdef DEBUG
  printf ("\nStart\tx = %d\ty = %d\n", x, y);
  #endif

  #ifdef DISPLAY
  startx = x;
  starty = y;
  #endif

  err = findpath (x, y, 57, 12);
  if (err == -1)
    {
      #ifdef DEBUG
      _perror ("findpath");
      #endif

      exit (EXIT_FAILURE);
    }

  while (!(err = nextstep (&x, &y)))
    {
      #ifdef DEBUG
      printf ("Next \tx = %d\ty = %d\n", x, y);
      #endif
    }

  #ifdef DISPLAY
  paths ();
  pthread_join (tid, NULL);
  #endif

  return EXIT_SUCCESS;
}

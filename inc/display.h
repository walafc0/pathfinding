#ifndef _DISPLAY_H_
#define _DISPLAY_H_

  #include "findpath.h"
  #include <pthread.h>
  #include <SDL/SDL.h>

  #define SCREEN screen
  #define SCALE  8
  #define DH     MAPH * SCALE
  #define DW     MAPW * SCALE * 4

  #define WHITE  (SDL_MapRGB (SCREEN -> format, 211, 215, 207))
  #define BLACK  (SDL_MapRGB (SCREEN -> format,  46,  52,  54))
  #define YELLOW (SDL_MapRGB (SCREEN -> format,  52, 101, 164))
  #define BLUE   (SDL_MapRGB (SCREEN -> format,  196, 160,  0))
  #define RED    (SDL_MapRGB (SCREEN -> format,  204,   0,  0))
  #define GREEN  (SDL_MapRGB (SCREEN -> format,   78, 154,  6))

  extern int startx;
  extern int starty;

  extern pthread_t tid;
  extern pthread_mutex_t mutex;
  extern pthread_cond_t  cond;

  void *display   (void *); 
  void  putPixel  (Uint16, Uint16, Uint32);
  void  draw      (Uint16, Uint16, Uint32);
  void  obstacles (void);
  void  lists     (void);
  void  paths     (void);
  void  segments  (Point, Point);
  void  update    (void);

#endif

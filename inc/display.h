#ifndef _DISPLAY_H_
#define _DISPLAY_H_

    #include <pthread.h>
    #include <SDL/SDL.h>
    #include "findpath.h"
    #include "local.h"

    #define ECRAN ecran
    #define SCALE 8
    #define DH    MAPH * SCALE
    #define DW    MAPW * SCALE * 4

    #define WHITE  ( SDL_MapRGB ( ECRAN -> format, 211, 215, 207 ) )
    #define BLACK  ( SDL_MapRGB ( ECRAN -> format,  46,  52,  54 ) )
    #define YELLOW ( SDL_MapRGB ( ECRAN -> format,  52, 101, 164 ) )
    #define BLUE   ( SDL_MapRGB ( ECRAN -> format,  196, 160,  0 ) )
    #define RED    ( SDL_MapRGB ( ECRAN -> format,  204,   0,  0 ) )
    #define GREEN  ( SDL_MapRGB ( ECRAN -> format,   78, 154,  6 ) )

    extern int startx;
    extern int starty;
    extern pthread_t       thread;
    extern pthread_mutex_t lock;

    void *display ();
    void  update ();
    void  obstacles ();
    void  lists ();
    void  paths ();
    void  segments ( Point a, Point b );
    
    void draw ( Uint16 x, Uint16 y, Uint32 color );
    void putPixel ( Uint16 x, Uint16 y, Uint32 color );

#endif

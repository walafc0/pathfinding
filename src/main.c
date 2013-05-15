#ifdef LOCAL
#include "local.h"
#endif

#ifdef DEBUG
#include "debug.h"
#endif

#ifdef DISPLAY
#include "display.h"
int startx = -1;
int starty = -1;
#endif

#include "errno.h"
#include "findpath.h"
#include "nextstep.h"

int
main ( void )
{
    #ifdef DISPLAY
    int inited = FALSE;
    pthread_mutex_init ( &lock, NULL );
    pthread_create ( &thread, NULL, display, &inited );
    while ( !inited );
    obstacles ();
    #endif

    // Premier déplacement :
    x = getX ();
    y = getY ();

    #ifdef DISPLAY
    startx = x;
    starty = y;
    #endif

    #ifdef DEBUG
    printf ( "\033[1mStart\tx = %d\ty = %d\033[0m\n", x, y );
    #endif

    errno = findpath ( x, y, 13, 37 );
    if ( !errno )
    {
        while ( !( errno = nextstep ( &x, &y ) )  )
        {
            #ifdef DEBUG
            printf ( "Next \tx = %d\ty = %d\n", x, y );
            #endif
            // move ( x, y );
        }

        #ifdef DISPLAY
        paths ();
        #endif
    }
    else
    {
        #ifdef DEBUG
        _perror ( "findpath" );
        #endif
    }
    //

    // Second déplacement :
    x = getX ();
    y = getY ();

    #ifdef DISPLAY
    startx = x;
    starty = y;
    #endif

    #ifdef DEBUG
    printf ( "\n\033[1mStart\tx = %d\ty = %d\033[0m\n", x, y );
    #endif

    errno = findpath ( x, y, 57, 12 );
    if ( !errno )
    {
        while ( !( errno = nextstep ( &x, &y ) )  )
        {
            #ifdef DEBUG
            printf ( "Next \tx = %d\ty = %d\n", x, y );
            #endif
            // move ( x, y );
        }

        #ifdef DISPLAY
        paths ();
        #endif
    }
    else
    {
        #ifdef DEBUG
        _perror ( "findpath" );
        #endif
    }
    //

    #ifdef DISPLAY
    pthread_join ( thread, NULL );
    #endif

    return 0;
}

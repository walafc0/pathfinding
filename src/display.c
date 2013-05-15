#include "display.h"
#include "findpath.h"
#include <pthread.h>
#include <SDL/SDL.h>

int startx = -1;
int starty = -1;

pthread_t tid;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

SDL_Surface *SCREEN;

void
*display (void *arg)
{
  // Initialisation :
  SDL_Init (SDL_INIT_VIDEO);
  SCREEN = SDL_SetVideoMode (DW, DH, 32, SDL_HWSURFACE);
  SDL_FillRect (SCREEN, NULL, WHITE);
  SDL_Flip (SCREEN);
    
  pthread_mutex_lock (&mutex);
  pthread_cond_signal (&cond);
  pthread_mutex_unlock (&mutex);

  SDL_Event event;
  while (TRUE)
    {
      SDL_WaitEvent (&event);
      switch (event.type)
        {
	case SDL_QUIT:
	  return NULL;
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      return NULL;
	    default:
	      break;
	    }
	  break;
	default:
	  break;
        }
    }
  return NULL;
}

void
obstacles ()
{
  for (int x = 0; x < MAPW; x++)
    {
      for (int y = 0; y < MAPH; y++)
        {
	  for (int z = 0; z < 4; z++)
            {
	      switch ((map[y][x] >> (6 - z * 2)) & 0x03)
                {
		case FREE:
		  draw (x * 4 + z, y, WHITE);
		  break;
		case OBSTACLE:
		  draw (x * 4 + z, y, BLACK);
		  break;
		default:
		  break;
                }
            }
        }
    }
}

void
lists ()
{
  for (int i = 0; i < openedNodes.number; i++)
    draw (openedNodes.node[i] -> x, openedNodes.node[i] -> y, YELLOW);

  for (int i = 0; i < closedNodes.number; i++)
    draw (closedNodes.node[i] -> x, closedNodes.node[i] -> y, BLUE);
    
  // Mise à jour de l'écran :
  SDL_Flip (SCREEN);
}

void
paths ()
{
  for (int i = 0; i < pathlength; i++)
    draw (path[i].x, path[i].y, RED);
    
  Point start = { startx, starty };
  segments (start, path_[0]);
  for (int i = 0; i < pathlength_ - 1; i++)
    segments (path_[i], path_[i+1]);
    
  // Mise à jour de l'écran :
  SDL_Flip (SCREEN);
}

void
segments (Point a, Point b)
{
  int x = a.x;
  int y = a.y;
  int dx = b.x - x;
  int dy = b.y - y;
  int xinc = (dx > 0) ? 1 : -1;
  int yinc = (dy > 0) ? 1 : -1;

  dx = (dx > 0) ? dx : -dx;
  dy = (dy > 0) ? dy : -dy;

  if (dx > dy)
    {
      int cumul = dx / 2;
      for (int i = 0; i < dx; i++)
	{
	  x += xinc;
	  cumul += dy;
	  if (cumul >= dx)
	    {
	      y += yinc;
	      cumul -= dx;
	    }
	  draw (x, y, GREEN);
	}
    }
  else
    {
      int cumul = dy / 2;
      for (int i = 0; i < dy; i++)
	{
	  y += yinc;
	  cumul += dx;
	  if (cumul >= dy)
	    {
	      x += xinc;
	      cumul -= dy;
	    }
	  draw (x, y, GREEN);
	}
    }
}

void
draw (Uint16 x, Uint16 y, Uint32 color)
{
  for (int i = 0; i < SCALE; i++)
    {
      for (int j = 0; j < SCALE; j++)
        {
	  putPixel ((x * SCALE + i), (y * SCALE + j), color);
        }
    }
}

void
putPixel (Uint16 x, Uint16 y, Uint32 color)
{
  int bytesperpixel = SCREEN -> format -> BytesPerPixel;
  Uint8 *p = ((Uint8 *) SCREEN -> pixels) + y * SCREEN -> pitch + x * bytesperpixel;

  switch (bytesperpixel)
    {
    case 1:
      *p = (Uint8) color;
      break;
    case 2:
      * ((Uint16 *) p) = (Uint16) color;
      break;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
	  * ((Uint16 *) p) = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
	  * (p + 2) = color & 0xff;
	}
      else
	{
	  * ((Uint16 *) p) = color & 0xffff;
	  * (p + 2) = (color & 0xff);
	}
      break;
    case 4:
      * ((Uint32 *) p) = color;
      break;
    }
}

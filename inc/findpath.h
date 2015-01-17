#ifndef _FINDPATH_H_
#define _FINDPATH_H_

#if defined(__unix__)
#include <sys/param.h>
#endif

  #include "map.h"

  #ifndef TRUE
  #define TRUE  42
  #endif

  #ifndef FALSE
  #define FALSE 0
  #endif

  #ifndef NULL
  #define NULL (void *) 0
  #endif

  #define MAXN       (MAPH * MAPW * 4) / 4    // Arbitraire
  #define COST(x, y) (map [(y)][(x) / 4] >> (6 - (((x) % 4) * 2)) & 0x03)

  typedef struct _Point Point;
  struct _Point
    {
      int x;
      int y;
    };

  typedef struct _Node Node;
  struct _Node
    {
      Node *parent;
      int x;
      int y;
      int opened;  // Booléen
      int closed;  // Booléen
      int G;       // Coût départ > noeud
      int H;       // Coût noeud  > arrivée
      int F;       // Somme des coûts G et H
    };

  typedef struct _List List;
  struct _List
    {
      int   number;
      Node *node[MAXN];  // Tableau de pointeurs
    };

  extern List openedNodes;
  extern List closedNodes;

  typedef Point Path[MAXN];
  extern Path path;
  extern int pathlength;
  extern Path path_;
  extern int pathlength_;

  typedef Node Graph[MAPH][MAPW<<4];
  extern Graph graph;

  int  findpath (int, int, int, int);
  void makepath (Node *, int, int);
  int  walkable (Point, Point);

  void addOpenedNodes (Node *);
  void delOpenedNodes (Node *);
  void addClosedNodes (Node *);
#if defined(BSD)
  void _heapsort (void);
#else
  void heapsort (void);
#endif
  void swap (int, int);
  void sift (int, int);
  int  heuristic (int, int, int, int);
    
#endif

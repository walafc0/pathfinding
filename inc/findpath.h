#ifndef _FINDPATH_H_
#define _FINDPATH_H_

    #ifdef DEBUG
    #include <stdio.h>
    #endif
    
    typedef struct _Point Point;
    struct _Point
    {
        int x;
        int y;
    };
    
    #ifdef DISPLAY
    #include <unistd.h>
    #include "display.h"
    #endif
    
    #include "errno.h"
    #include "config.h"
    #include "map.h"
    
    #ifndef TRUE
    #define TRUE 1
    #endif
    
    #ifndef FALSE
    #define FALSE 0
    #endif
    
    #ifndef NULL
    #define NULL ( void * ) 0
    #endif
    
    #define MAXN       ( MAPH * MAPW * 4 ) / 4    // Arbitraire
    #define COST(x, y) ( map [ ( y ) ] [ ( x ) / 4 ] >> ( 6 - ( ( ( x ) % 4 ) * 2 ) ) & 0x03 )
    
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
    
    int  findpath  ( int x, int y, int x_, int y_ ); // TODO: Pourquoi A* ne donne pas le plus court chemin ?
    void makepath ( Node* node, int x, int y );
    int  walkable ( Point a, Point b );
    
    // TODO: inline
    void addOpenedNodes ( Node *node );
    void delOpenedNodes ( Node *node );
    void addClosedNodes ( Node *node );
    void heapsort ();
    void swap ( int a, int b );
    void sift ( int parent_index, int height );
    int  heuristic ( int x, int y, int x_, int y_ ); // TODO: Vérifier le choix de l'heuristique
    
#endif

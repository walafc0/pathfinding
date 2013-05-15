#define _XOPEN_SOURCE 600

#include "findpath.h"

Map   map = MAP;
Graph graph;
List  openedNodes;
List  closedNodes;

int pathlength;
Path path;
int pathlength_;
Path path_;

int
findpath ( const int x, const int y, const int x_, const int y_ )
{
    #ifdef DEBUG
    printf ( "\033[1mEnd  \tx = %d\ty = %d\033[0m\n", x_, y_ );
    #endif

    // Initialistaion des listes :
    openedNodes.number = 0;
    closedNodes.number = 0;
    
    // Initialisation des chemins :
    pathlength  = 0;
    pathlength_ = 0;

    // Vérification du cas où le point de départ est le point d'arrivé serait le même... :
    if ( x == x_
      && y == y_ )
        return _E_SAME;

    // Vérification du cas où le point de départ n'est pas dans les bornes de la carte :
    if ( x < 0
      || x > ( ( MAPW * 4 ) - 1 )
      || y < 0
      || y > ( MAPH - 1 ) )
        return _E_OUT;

    // Vérification du cas où le point de départ est un obstacle :
    if ( COST ( x, y ) == OBSTACLE )
        return _E_OBS;

    // Vérification du cas où le point d'arrivée n'est pas dans les bornes de la carte :
    if ( x_ <  0
      || x_ > ( ( MAPW * 4 ) - 1 )
      || y_ <  0
      || y_ > ( MAPH - 1 ) )
        return _E_OUT_;

    // Vérification du cas où le point d'arrivée est un obstacle :
    if ( COST ( x_, x_ ) == OBSTACLE )
        return _E_OBS_;
    
    // Initialisation du graphe :
    for ( int y = 0; y < MAPH; y++ )
    {
        for ( int x = 0; x < MAPW * 4; x++ )
        {
            graph[y][x].parent = NULL;
            graph[y][x].x      = x;
            graph[y][x].y      = y;
            graph[y][x].opened = FALSE;
            graph[y][x].closed = FALSE;
            graph[y][x].G      = 0;
            graph[y][x].H      = 0;
            graph[y][x].F      = 0;
        }
    }
    
    // Initialisation du premier noeud :
    Node *current = &(graph[y][x]);
    current -> x = x;
    current -> y = y;
    current -> H = heuristic ( x, y, x_, y_ );
    current -> F = current -> H;
    
    addOpenedNodes ( current );
    
    int loop = 0;
    while ( openedNodes.number )
    {
        // S'il y a déjà eu trop de calculs :
        if ( loop > MAX_LOOP )
             return _E_LOOP;
        else
            loop++;
        
        // Récupération du noeud ayant le plus petit cout cumulé :
        heapsort();
        current = openedNodes.node[0];
        
        // Si le noeud courant est le noeud d'arrivée :
        if ( current -> x == x_
          && current -> y == y_ )
        {
            makepath ( current, x, y );
            return _E_OK;
        }
        
        #ifdef DEBUG
        // printf ( "Current:\tx = %d\ty = %d\n", current -> x, current -> y );
        #endif
        
        delOpenedNodes ( current );
        addClosedNodes ( current );
        
        int xoffset[] = { -1,  0,  1, -1,  1, -1,  0,  1 };
        int yoffset[] = { -1, -1, -1,  0,  0,  1,  1,  1 };
        for ( int i = 0; i < 8; i++ )
        {
            int xneighbor = current -> x + xoffset[i];
            int yneighbor = current -> y + yoffset[i];
            
            if ( xneighbor <  0
              || xneighbor > ( ( MAPW * 4 ) - 1 )
              || yneighbor <  0
              || yneighbor > ( MAPH - 1 ) )
                continue;

            if ( COST ( xneighbor, yneighbor ) == OBSTACLE )
                continue;
            
            Node *neighbor = &(graph[yneighbor][xneighbor]);
            
            if ( neighbor -> closed )
                continue;
            
            #ifdef DEBUG
            // printf ( " Neighbor:\tx = %d\ty = %d\n", neighbor -> x, neighbor -> y );
            #endif
            
            int G = current -> G + COST ( neighbor -> x, neighbor -> y );
            int H = heuristic ( neighbor -> x, neighbor -> y, x_, y_ );
            int F = G + H;
            
            if ( neighbor -> opened )
            {
                if ( F < current -> F )
                {
                    neighbor -> G = G;
                    neighbor -> H = H;
                    neighbor -> F = F;
                    neighbor -> parent = current;
                }
            }
            else
            {
                neighbor -> G = G;
                neighbor -> H = H;
                neighbor -> F = F;
                neighbor -> parent = current;
                
                addOpenedNodes ( neighbor );
            }
        }
        
        #ifdef DEBUG
        // printf ( "\n" );
        #endif
        
        #ifdef DISPLAY
        lists ();
        usleep ( 5000 );
        #endif
    }

    // Aucun chemin n'existe :
    return _E_NOP;
}

void
addOpenedNodes ( Node *node )
{
    node -> opened = TRUE;
    openedNodes.node [ openedNodes.number++ ] = node;
}

void
delOpenedNodes ( Node *node )
{
    node -> opened = FALSE;
    openedNodes.number--;
    for ( int i = 0; i < openedNodes.number; i++ )
    {
        if ( openedNodes.node[i] -> x == node -> x
          && openedNodes.node[i] -> y == node -> y )
        {
            openedNodes.node[i] = openedNodes.node[openedNodes.number];
            break;
        }
    }
}
            
void
addClosedNodes ( Node *node )
{
    node -> closed = TRUE;
    closedNodes.node [ closedNodes.number++ ] = node;
}

void
makepath ( Node* node, int x, int y )
{
    pathlength = 0;

    // Tant que l'on est pas arrivé au point de départ
    while ( node -> x != x 
         || node -> y != y )
    {
        path[pathlength].x = node -> x;
        path[pathlength].y = node -> y;
        pathlength++;
        node = node -> parent;
    }
    
    // Line of Sight :
	if ( pathlength < 4 ) // Rien à faire
	{
		path_[pathlength_++] = path[0];
		return;
	}

	int endx       = path[0].x;
	int endy       = path[0].y;
	int firstpoint = pathlength - 1;
	int lastpoint  = firstpoint - 2;

	while ( path[lastpoint].x != endx
	     || path[lastpoint].y != endy )
	{
		if ( !walkable ( path[lastpoint], path[firstpoint] ) )
		{
			path_[pathlength_++] = path[lastpoint+1];
			firstpoint = lastpoint+1;
		}
		lastpoint--;
	}
	path_[pathlength_++] = path[lastpoint];
}

int
walkable ( Point a, Point b )
{
	int x = a.x;
	int y = a.y;
	int dx = b.x - x;
	int dy = b.y - y;
	int xinc = ( dx > 0 ) ? 1 : -1;
	int yinc = ( dy > 0 ) ? 1 : -1;

	dx = ( dx > 0 ) ? dx : -dx;
	dy = ( dy > 0 ) ? dy : -dy;

	if ( dx > dy )
	{
		int cumul = dx / 2;
		for ( int i = 0; i < dx; i++ )
		{
			x += xinc;
			cumul += dy;
			if ( cumul >= dx )
			{
				y += yinc;
				cumul -= dx;
			}
			if ( COST(x, y) == OBSTACLE )
				return FALSE;
		}
	}
	else
	{
		int cumul = dy / 2;
		for ( int i = 0; i < dy; i++ )
		{
			y += yinc;
			cumul += dx;
			if ( cumul >= dy )
			{
				x += xinc;
				cumul -= dy;
			}
			if ( COST(x, y) == OBSTACLE )
				return FALSE;
		}
	}
	return TRUE;
}

void
heapsort ()
{
    int root = openedNodes.number / 2 - 1;
    while ( root > -1 )
        sift ( root--, openedNodes.number );
    
    int end = openedNodes.number - 1;
    while ( end > 0 )
    {
        swap ( end, 0 );
        sift ( 0, end-- );
    }
}

void
sift ( int root, int size )
{
    int child  = -1;
    
    while ( ( child = root * 2 + 1 ) < size )
    {
        if ( child+1 < size )
            child = ( openedNodes.node[child+1] -> F > openedNodes.node[child] -> F ? child+1 : child );
        
        if ( !( openedNodes.node[root] -> F < openedNodes.node[child] -> F ) )
            return;
        
        swap ( root, child );
        root = child;
    }
}

void
swap ( int i, int j )
{
    Node *tmp = openedNodes.node[i];
    openedNodes.node[i] = openedNodes.node[j];
    openedNodes.node[j] = tmp;
}

int
heuristic ( int x, int y, int x_, int y_ )
{
    return ( x - x_ ) * ( x - x_ ) + ( y - y_ ) * ( y - y_ );
}

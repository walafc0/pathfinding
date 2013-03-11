#include "shared.h"
#include "findPath.h"

// TODO: Modifier les types de listes
// TODO: Supprimer `coordinates` de la structure `node` (on connait déjà les coordonnées pusiqu'on est dans une matrice)

extern Map map ;
extern Path path ;
extern u16 pathLength ;
extern Path pathSmoothed ;
extern u16 pathSmoothedLength ;
extern Heap open_list ;
extern Heap closed_list ;

Node matriceNoeuds[MAP_HEIGHT][MAP_WIDTH * 4] ;

u8 findPath ( u32 start, u32 end )
{
	// Vérification du point de départ :
	if ( COST( X(start), Y(start) ) == BUSY )
		return ERR_1;

	// Vérification du point d'arrivée :
	if ( COST( X(end), Y(end) ) == BUSY )
		return ERR_2;

	// Vérification du cas où le point de départ est le point d'arrivé serait le même... :
	if ( start == end )
		return ERR_3;

	// Vérification du cas où le point de départ n'est pas dans les bornes de la carte :
	if ( X(start) < 0
	  || X(start) > ( ( MAP_WIDTH * 4 ) - 1 )
	  || Y(start) < 0
	  || Y(start) > ( MAP_HEIGHT - 1 ) )
		return ERR_4;

	// Vérification du cas où le point d'arrivée n'est pas dans les bornes de la carte :
	if ( X(end) < 0
	  || X(end) > ( ( MAP_WIDTH * 4 ) - 1 )
	  || Y(end) < 0
	  || Y(end) > ( MAP_HEIGHT - 1 ) )
		return ERR_5;

	u8 indice ;
	s8 x[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	s8 y[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	u16 loop_number = 0;
	Node *current ;
	Node *neighbor ;

	// Initialisation des listes :
	open_list.count = 0 ;
	open_list.node[0].parent = NULL ;
	closed_list.count = 0 ;
	closed_list.node[0].parent = NULL ;

	// Initialisation de la matrice des noeuds :
	for ( u16 i = 0 ; i < MAP_HEIGHT ; i++ )
	{
		for ( u16 j = 0 ; j < MAP_WIDTH * 4 ; j++ )
		{
			matriceNoeuds[i][j].coordinates = COORD(j, i) ;
			matriceNoeuds[i][j].parent	= NULL ;
			matriceNoeuds[i][j].open_list	= 0 ;
			matriceNoeuds[i][j].closed_list = 0 ;
			matriceNoeuds[i][j].cost_g	= 0 ;
			matriceNoeuds[i][j].cost_h	= 0 ;
		}
	}

	// Initialisation du premier noeud :
	current = &matriceNoeuds[Y(start)][X(start)] ;
	current -> cost_h = heuristique(current, end) ;

	// Ajout à la liste ouverte :
	current -> open_list = TRUE ;
	open_list.node[open_list.count] = *current ;
	open_list.count++ ;

	while ( open_list.count )
	{
		if ( loop_number >= MAX_LOOP )
			return ERR_6;
		else
			loop_number++;

		// Récupération du noeud ayant le plus petit cout cumulé :
		heap_sort() ;
		u32 current_coordinates = open_list.node[0].coordinates ;

		current = &matriceNoeuds[Y(current_coordinates) ][X(current_coordinates)] ;

		// Ajout à la liste ouverte :
		current -> closed_list = TRUE ;
		closed_list.node[closed_list.count] = *current ;
		closed_list.count++ ;

		// Si le noeud courant est le noeud d'arrivée :
		if ( current -> coordinates == end )
		{
			make_path(current, start);
			return SUCCESS;
		}

		// Suppression de la liste des noeuds ouverts :
		current -> open_list = FALSE ;
		open_list.count-- ;
		for ( u16 i = 0 ; i < open_list.count ; i++ )
		{
			if ( open_list.node[i].coordinates == current -> coordinates )
			{
				open_list.node[i] = open_list.node[open_list.count] ;
				break ;
			}
		}

		for ( indice = 0 ; indice < 8 ; indice ++ )
		{
			u16 neighbor_x = (X(current -> coordinates)) + x[indice];
			u16 neighbor_y = (Y(current -> coordinates)) + y[indice];

			if ( neighbor_x >= MAP_WIDTH*4
			  || neighbor_y >= MAP_HEIGHT )
				continue ;

			neighbor = &matriceNoeuds[neighbor_y][neighbor_x] ;
			u8 cost_obstacle = COST(neighbor_x, neighbor_y);

			switch ( cost_obstacle )
			{
				case 0:
					cost_obstacle = SURE_COST ;
					break;
				case 1:
					cost_obstacle = FREE_COST ;
					break;
				case 2:
					cost_obstacle = RISQUY_COST ;
					break;
				default:
					continue ;
			}

			u16 cost = current -> cost_g + cost_obstacle ;

			if ( neighbor -> closed_list )
			{
				if ( neighbor -> cost_g > cost )
				{
					// Suppression de la liste des noeuds fermés :
					neighbor -> closed_list = FALSE ;
					closed_list.count-- ;
					for ( u16 i = 0 ; i < closed_list.count ; i++ )
					{
						if ( closed_list.node[i].coordinates == neighbor -> coordinates )
						{
							closed_list.node[i] = closed_list.node[closed_list.count] ;
							break ;
						}
					}

					neighbor -> parent = current ;
					neighbor -> cost_h = heuristique ( neighbor, end ) ;

					// Ajout à la liste ouverte :
					neighbor -> open_list = TRUE ;
					open_list.node[open_list.count] = *neighbor ;
					open_list.count++ ;
				}
				else
				{
					continue ;
				}
			}
			else if ( neighbor -> open_list )
			{
				if ( neighbor -> cost_g > cost )
				{
					neighbor -> cost_g = cost ;
					neighbor -> parent = current ;
					neighbor -> cost_h = heuristique(neighbor, end);
				}
			}
			else
			{
				neighbor -> cost_g = cost ;
				neighbor -> cost_h = heuristique(neighbor, end) ;
				neighbor -> coordinates = COORD(neighbor_x, neighbor_y) ;
				neighbor -> parent = current ;

				if ( !neighbor -> open_list )
				{
					// Ajout à la liste ouverte :
					neighbor -> open_list = TRUE ;
					open_list.node[open_list.count] = *neighbor ;
					open_list.count++ ;
				}
			}
		}
	}


	// Aucun chemin n'existe :
	return ERR_7 ;
}


/* FONCTIONS CONCERNANT LE CHEMIN */
void make_path ( Node* node, u32 start )
{
	pathLength = 0 ;
	pathSmoothedLength = 0;

	// Tant que l'on est pas arrivé au point de départ
	while (node -> coordinates != start)
	{
		path[pathLength] = node -> coordinates ;
		pathLength++ ;
		node = node -> parent ;
	}
	
	// Permet d'ajouter le point de départ dans l'affichage (à ne pas embarquer) :
	path[pathLength] = node -> coordinates ;
	pathLength++ ;

	/***** LISSAGE : *****/

	if ( pathLength < 4 ) // Rien à lisser
	{
		pathSmoothed[pathSmoothedLength++] = path[0];
		return;
	}

	u32 end = path[0] ;
	u16 firstPoint = pathLength - 1 ;
	u16 lastPoint = firstPoint - 2 ;

	while ( path[lastPoint] != end )
	{
		if ( !isWalkable(path[lastPoint], path[firstPoint]) )
		{
			pathSmoothed[pathSmoothedLength++] = path[lastPoint+1] ;
			firstPoint = lastPoint+1 ;
		}
		lastPoint-- ;
	}
	pathSmoothed[pathSmoothedLength++] = path[lastPoint];
}

u8 isWalkable(u32 a, u32 b)
{
	u16 x = X(a);
	u16 y = Y(a)  ;
	s16 dx = X(b) - x ;
	s16 dy = Y(b) - y ;
	s8 xinc = ( dx > 0 ) ? 1 : -1 ;
	s8 yinc = ( dy > 0 ) ? 1 : -1 ;

	dx = ( dx > 0 ) ? dx : -dx ;
	dy = ( dy > 0 ) ? dy : -dy ;

	if ( dx > dy )
	{
		s16 cumul = dx / 2 ;
		for ( u16 i = 0 ; i < dx ; i++ )
		{
			x += xinc ;
			cumul += dy ;
			if ( cumul >= dx )
			{
				y += yinc ;
				cumul -= dx ;
			}
			if ( COST(x, y) == BUSY )
				return FALSE ;
		}
	}
	else
	{
		s16 cumul = dy / 2 ;
		for ( u16 i = 0 ; i < dy ; i++ )
		{
			y += yinc ;
			cumul += dx ;
			if ( cumul >= dy )
			{
				x += xinc ;
				cumul -= dy ;
			}
			if ( COST(x, y) == BUSY )
				return FALSE ;
		}
	}
	return TRUE;
}

/* FONCTIONS CONCERNANT LES TAS */
void swap ( u16 a, u16 b )
{
	Node tmp = open_list.node[a] ;
	open_list.node[a] = open_list.node[b] ;
	open_list.node[b] = tmp ;
}

void sift ( u16 parent_index, u16 height )
{
	u16 index = 2 * parent_index;

	while ( index <= height )
	{
		u16 first_son_cost = open_list.node[index].cost_g + open_list.node[index].cost_h ;
		u16 second_son_cost	= open_list.node[index+1].cost_g + open_list.node[index+1].cost_h ;
		u16 parent_cost = open_list.node[parent_index].cost_g + open_list.node[parent_index].cost_h ;

		if ( index < height && first_son_cost < second_son_cost )
		{
			index++ ;
			first_son_cost = second_son_cost;
		}

		if ( parent_cost < first_son_cost )
		{
			swap ( parent_index, index ) ;
			parent_index = index ;
			index = 2 * parent_index ;
		}
		else
		{
			break ;
		}
	}
}

void heap_sort ()
{
	u16 i ;
	u16 height = open_list.count ;

	for ( i = height/2 ; i > 0 ; i-- )
	{
		sift ( i-1, height ) ;
	}

	for ( i = height ; i > 1 ; i-- )
	{
		swap ( i-1, 0 ) ;
		for ( u16 k = i/2 ; k > 0 ; k-- )
		{
			sift ( k-1, i-2 ) ;
		}
	}
}

/* FONCTIONS QUI CONCERNENT LES HEURISTIQUES */
u16 heuristique ( Node *current, u32 end )
{
	u16 dx = SABS(X(current -> coordinates), X(end)) ;
	u16 dy = SABS(Y(current -> coordinates), Y(end)) ;
	u16 heuristique =  DIAGONAL * (dx > dy ? dy : dx) + STRAIGHT * SABS(dx, dy) ;
	return heuristique ;
}

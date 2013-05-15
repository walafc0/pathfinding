#ifndef _CONFIG_H_
	#define _CONFIG_H_

	#include	"./map.h"			// Header contenant la carte utilisée

	#define		MAX_LOOP		5000	// Nombre max. de boucle dans la fonction findPath()

	#define		MAX_PAWN		10	// Nombre max. de poin déposés sur le plateau ( < 256 )
	#define		TICKS_TO_UPDATE		100	// Nombre de ticks avant màj de la carte par l'appel de update()

	#define		SURE_COST		1	// Cout pour le type d'obstacle "sure"
	#define		FREE_COST		2	// Cout pour le type d'obstacle "libre"
	#define		RISQUY_COST		3	// Cout pour le type d'obstacle "risqué"
							// Pour tous les coûts : mettre à 0 pour considérer comme infranchissable
#endif

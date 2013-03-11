/*
 * affichage.h
 *
 *  Created on: 9 févr. 2011
 *      Author: E094057L
 */

#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

	#include <stdlib.h>
	#include <stdio.h>
	#include <unistd.h>
	#include <SDL/SDL.h>
	#include "shared.h"

	#define ECHELLE 3
	#define HAUTEUR_ECRAN MAP_HEIGHT * ECHELLE
	#define LARGEUR_ECRAN MAP_WIDTH * 4 * ECHELLE

	#define BLANC(x)	(SDL_MapRGB(x->format, 255, 255, 255))

	#define CHEMIN(x)	(SDL_MapRGB(x->format, 204, 0, 0))	// ROUGE
	#define RES(x)		(SDL_MapRGB(x->format, 0, 240, 0))		// VERT
	#define OUVERT(x)	(SDL_MapRGB(x->format, 52, 101, 164))	// JAUNE
	#define FERME(x)	(SDL_MapRGB(x->format, 196, 160, 0))	// BLEU

	#define LOL(x)		(SDL_MapRGB(x->format, 78, 154, 6))
	#define ARMEL(x)	(SDL_MapRGB(x->format, 200, 10, 50))

	// Nuances de gris pour les obstacles
	#define SUR(x)		(SDL_MapRGB(x->format, 240, 240, 240))
	#define LIBRE(x) 	(SDL_MapRGB(x->format, 211, 215, 207))
	#define RISQUE(x) 	(SDL_MapRGB(x->format, 90, 90, 90))
	#define OCCUPE(x)	(SDL_MapRGB(x->format, 46,  52, 54))

	/* Appli principale */
	int affichage();

	/* Put pixel permet de dessiner un pixel aux coordonnées (x,y) avec une couleur définie */
	void putPixel(Uint16 x, Uint16 y, Uint32 color);

	/* Permet de dessiner un carré de pixels dont la taille change en fonction de l'échelle */
	void dessiner(Uint16 x, Uint16 y, Uint32 color);

	/* Affichage de la map avec les obstacles calculés */
	int affichageobstacles();

	/* Affichage de la map sans les obstacles calculés */
	int affichageobstaclesBis();

	/* Affichage de la liste ouverte et de la liste fermée */
	int affichagelistes();

	/* Affichage du chemin */
	int affichagechemin();

	/* Affichage des points renvoyés */
	void affichageres();
	void affichagesegment();

	/* Lol */
	void easterEgg();

	/* Dessine les segments qui ont été calculés lors du smooth (tracerSegment @ findpath.c) */
	//int dessinerSegment();
	void dessinerSegment(int x1, int y1, int x2, int y2);


#endif /* AFFICHAGE_H_ */

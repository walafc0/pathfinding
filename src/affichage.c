/* ***** DOCUMENTATION SUR LE SIMULATEUR *****
 *
 * FLECHE GAUCHE/DROITE :
 * Montre la map avec les obstacles représentés par des nuances de gris.
 * - En rouge le chemin calculé par findPath()
 * - En vert les points calculés par smooth(). Ce sont les points qui seront renvoyés au robot lorsque notre programme sera appelé
 *
 * FLECHE GAUCHE : affiche la même chose que flèche droite avec des obstacles plus gros.
 * - En bleu : les zones où le robot ne peut pas aller à cause de sa taille. La taille de ces zones est redéfinissable dans l'éditeur de maps
 *
 *
 * FLECHE HAUT :
 * Montre l'état des listes ouvertes et fermées à la fin de l'exécution du programme
 * - En bleu la liste fermée
 * - En jaune la liste ouverte
 * - On affiche en plus les points verts renvoyés au robot
 *
 */

#include "shared.h"
#include "affichage.h"

extern Map map;
extern Map mapBis;
extern Path path;
extern u16 pathLength;
extern Path pathSmoothed;
extern u16 pathSmoothedLength;
extern Heap open_list;
extern Heap closed_list;

SDL_Surface *ecran;

// A renommer en affichage() après l'intégration au code
int affichage()
{
	// Initialisation...
	//SDL_Surface *ecran = NULL;
	ecran = NULL;
	SDL_Init(SDL_INIT_VIDEO);

	// Création de la fenêtre
	//putenv("SDL_VIDEO_WINDOW_POS=center");
	ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Simulator - LEFT/RIGHT pour le chemin, UP pour l'état des listes", NULL);

	// On remplit l'écran de blanc
	SDL_FillRect(ecran, NULL, BLANC(ecran));

	/*
	 * Affichage par défaut à l'ouverture
	 */
	affichageobstaclesBis();
	affichagesegment();
	affichageres();
	SDL_Flip(ecran);

	SDL_Event event;
	int continuer = 1;
	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_UP:
						affichageobstaclesBis();
						affichagesegment();
						affichageres();
						break;
					case SDLK_DOWN:
						easterEgg();
						break;
					case SDLK_RIGHT:
						affichageobstacles();
						affichagelistes();
						affichagechemin();
						break;
					case SDLK_LEFT:
						affichageobstaclesBis();
						affichagelistes();
						affichagechemin();
						affichagesegment();
						break;
					case SDLK_ESCAPE:
						continuer = 0;
						break;
					default:
						break;
				}
				break;
		}
		SDL_Flip(ecran);
	}

	SDL_Quit();
	return EXIT_SUCCESS;
}


int affichagelistes()
{
	SDL_WM_SetCaption("Simulator - LEFT/RIGHT pour le chemin, UP pour l'état des listes (ouvert : jaune  ;  fermé : bleu)", NULL);

	// Affichage de la liste ouverte :
	u16 i, x, y;
	u32 coords;

	for(i = 0 ; i < open_list.count ; i++)
	{
		coords = open_list.node[i].coordinates;
		x = X(coords);
		y = Y(coords);
		dessiner(x,y,OUVERT(ecran));
	}
	for(i = 0 ; i < closed_list.count ; i++)
	{
		coords = closed_list.node[i].coordinates;
		x = X(coords);
		y = Y(coords);
		dessiner(x,y,FERME(ecran));
	}
	return EXIT_SUCCESS;
}


int affichageobstacles()
{
	SDL_WM_SetCaption("Simulator - LEFT/RIGHT pour le chemin, UP pour l'état des listes", NULL);

	// On remplit l'écran de blanc
	SDL_FillRect(ecran, NULL, BLANC(ecran));

	// Analyse de la matrice
	int i,j;
	for(i = 0 ; i < MAP_HEIGHT ; i++)
	{
		for(j = 0 ; j < MAP_WIDTH ; j++)
		{
			// On récupère la map octet par octet
			u8 octet = map[i][j];
			u8 octetBis = mapBis[i][j];

			// 1 octet représentant 4 obstacles, on boucle pour les récupérer un par un
			int k;
			for(k = 0 ; k < 4 ; k++)
			{
				u8 obstacle = ((octet >> (6-k*2)) & 0x03);
				u8 obstacleBis = ((octetBis >> (6-k*2)) & 0x03);

				switch (obstacle)
				{
					case 0x00:	// zone sûre
						dessiner(j*4+k,i, SUR(ecran));
						break;
					case 0x01:	// zone libre
						dessiner(j*4+k,i, LIBRE(ecran));
						break;
					case 0x02:	// zone risquée
						dessiner(j*4+k,i, RISQUE(ecran));
						break;
					case 0x03: // zone occupée
						if ( obstacle != obstacleBis )
							dessiner(j*4+k,i, ARMEL(ecran));
						else
							dessiner(j*4+k,i, OCCUPE(ecran));
						break;
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

int affichageobstaclesBis()
{
	SDL_WM_SetCaption("Simulator - LEFT/RIGHT pour le chemin, UP pour l'état des listes", NULL);

	// On remplit l'écran de blanc
	SDL_FillRect(ecran, NULL, BLANC(ecran));

	// Analyse de la matrice
	int i,j;
	for(i = 0 ; i < MAP_HEIGHT ; i++)
	{
		for(j = 0 ; j < MAP_WIDTH ; j++)
		{
			// On récupère la map octet par octet
			u8 octet = mapBis[i][j];

			// 1 octet représentant 4 obstacles, on boucle pour les récupérer un par un
			int k;
			for(k = 0 ; k < 4 ; k++)
			{
				u8 obstacle = ((octet >> (6-k*2)) & 0x03);

				switch(obstacle)
				{
					case 0x00:	// zone sûre
					{
						dessiner(j*4+k,i, SUR(ecran));
						break;
					}
					case 0x01:	// zone libre
					{
						dessiner(j*4+k,i, LIBRE(ecran));
						break;
					}
					case 0x02:	// zone risquée
					{
						dessiner(j*4+k,i, RISQUE(ecran));
						break;
					}
					case 0x03: // zone occupée
					{
						dessiner(j*4+k,i, OCCUPE(ecran));
						break;
					}
				}

			}
		}
	}
	return EXIT_SUCCESS;
}


int affichagechemin()
{
	for ( u16 i = 0 ; i < pathLength ; i++ )
	{
		int x = X(path[i]);
		int y = Y(path[i]);

		dessiner(x,y, CHEMIN(ecran));
	}
	return EXIT_SUCCESS;
}

void affichagesegment()
{
	for ( u16 i = 0 ; i < pathSmoothedLength ; i++)
	{
		int x1;
		int y1;

		if (i > 0)
		{
			x1 = X(pathSmoothed[i-1]);
			y1 = Y(pathSmoothed[i-1]);
		}
		else
		{
			x1 = X(path[pathLength-1]);
			y1 = Y(path[pathLength-1]);
		}

		int x2 = X(pathSmoothed[i]);
		int y2 = Y(pathSmoothed[i]);

		dessinerSegment(x1, y1, x2, y2);
	}
}

void affichageres()
{
	dessiner(X(path[pathLength-1]), Y(path[pathLength-1]), CHEMIN(ecran));

	for ( u16 i = 0 ; i < pathSmoothedLength ; i++)
		dessiner(X(pathSmoothed[i]), Y(pathSmoothed[i]), RES(ecran));
}

void dessiner(Uint16 x, Uint16 y, Uint32 color)
{
	int i,j;

	for(i = 0 ; i < ECHELLE ; i++)
	{
		for(j = 0 ; j < ECHELLE ; j++)
		{
			putPixel((x * ECHELLE + i), (y * ECHELLE + j), color);
		}
	}
}

void dessinerSegment(int x1, int y1, int x2, int y2)
{
	u32 chemin[1000];
	u32 p1 = COORD(x1, y1);
	u32 p2 = COORD(x2, y2);
	u16 length = 0;

	int dx,dy,i,xinc,yinc,cumul,x,y ;

	x = X(p1);
	y = Y(p1)  ;
	dx = X(p2) - X(p1) ;
	dy = Y(p2) - Y(p1) ;
	xinc = ( dx > 0 ) ? 1 : -1 ;
	yinc = ( dy > 0 ) ? 1 : -1 ;
	dx = ( dx > 0 ) ? dx : -dx ;
	dy = ( dy > 0 ) ? dy : -dy ;

	chemin[length] = COORD(x, y) ;
	length++ ;
	if ( dx > dy )
	{
		cumul = dx / 2 ;
		for ( i = 1 ; i <= dx ; i++ )
		{
			x += xinc ;
			cumul += dy ;
			if (cumul >= dx)
			{
				cumul -= dx ;
				y += yinc ;
			}
			chemin[length] = COORD(x, y) ;
			length++ ;
		}
	}
	else
	{
		cumul = dy / 2 ;
		for ( i = 1 ; i <= dy ; i++ )
		{
			y += yinc ;
			cumul += dx ;
			if ( cumul >= dy )
			{
				cumul -= dy ;
				x += xinc ;
			}
			chemin[length] = COORD(x, y) ;
			length++ ;
		}
	}

	for ( int i = 0; i < length; i++ )
	{
		dessiner(X(chemin[i]),Y(chemin[i]), LOL(ecran));
	}
}

void putPixel(Uint16 x, Uint16 y, Uint32 color)
{
	/* Nombre de bits par pixels de la surface d'écran */
	Uint8 bpp = ecran->format->BytesPerPixel;

	/* Pointeur vers le pixel à remplacer (pitch correspond à la taille
	d'une ligne d'écran, c'est à dire (longueur * bitsParPixel)
	pour la plupart des cas) */
	Uint8 * p = ((Uint8 *)ecran->pixels) + y * ecran->pitch + x * bpp;

	switch(bpp)
	{
		case 1:
			*p = (Uint8) color;
			break;
		case 2:
			*(Uint16 *)p = (Uint16) color;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				*(Uint16 *)p = ((color >> 8) & 0xff00) | ((color >> 8) & 0xff);
				*(p + 2) = color & 0xff;
			}
			else
			{
				*(Uint16 *)p = color & 0xffff;
				*(p + 2) = (X(color) & 0xff) ;
			}
			break;
		case 4:
			*(Uint32 *)p = color;
			break;
	}
}

#define NB_JACKIE 5

void easterEgg()
{
	int xinc[NB_JACKIE];
	int yinc[NB_JACKIE];
	int nbJackie = NB_JACKIE;
	int count = 0;

	SDL_Event event;
	SDL_Rect position[nbJackie];
	SDL_Surface *jackie = jackie = SDL_LoadBMP("./.jackiechan.bmp");

	for (int i = 0; i < nbJackie; i++)
	{
		position[i].x = rand() % (ecran->w - jackie->h);
		position[i].y = rand() % (ecran->h - jackie->h);
		xinc[i] = rand() % (20) + 5;
		yinc[i] = rand() % (20) + 5;
	}

	SDL_WM_SetCaption("Easter Egg - Yackiiiii (Chan)!!", NULL);

	while (1)
	{
		if ( SDL_PollEvent (&event)
		  && event.type == SDL_KEYDOWN )
			break;

		switch (count)
		{
			case 0:
				SDL_FillRect(ecran, NULL, (SDL_MapRGB(ecran->format, 255, 0, 0)));
				count++;
				break;
			case 1:
				SDL_FillRect(ecran, NULL, (SDL_MapRGB(ecran->format, 0, 255, 0)));
				count++;
				break;
			case 2:
				SDL_FillRect(ecran, NULL, (SDL_MapRGB(ecran->format, 0, 0, 255)));
				count++;
				break;
			default:
				SDL_FillRect(ecran, NULL, (SDL_MapRGB(ecran->format, 250, 250, 0)));
				count = 0;
				break;
		}

		for (int i = 0; i < nbJackie; i++)
		{
			position[i].x += xinc[i];
			position[i].y += yinc[i];

			if ( position[i].y + jackie->h > ecran->h
			  || position[i].y <= 0 )
			{
				yinc[i] *= -1;
			}
			if ( position[i].x + jackie->w > ecran->w
			  || position[i].x <= 0 )
			{
				xinc[i] *= -1;
			}
			SDL_BlitSurface(jackie, NULL, ecran, &position[i]);
		}
		SDL_Flip(ecran);
	}
	
	affichageobstaclesBis();
	affichagesegment();
	affichageres();
}

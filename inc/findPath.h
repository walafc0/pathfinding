#ifndef _FINDPATH_H_
	#define _FINDPATH_H_

	#define	DIAGONAL	14
	#define	STRAIGHT	10

	#define	SABS(x, y)	((x) > (y) ? ((x) - (y)) : ((y) - (x)))

	// Fonction permettant de recréer le chemin
	void make_path	( Node* end, u32 start ) ;
	u8 isWalkable	( u32 a, u32 b ) ;

	//Fonctions concernant le tas
	void heap_sort	() ;
	void sift 	( u16 a, u16 b ) ;
	void swap 	( u16 i, u16 j ) ;

	// Fonctions concernant l'heuristique
	u16 heuristique ( Node *current, u32 end ) ;

#endif

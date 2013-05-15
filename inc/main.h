#ifndef _MAIN_H_
	#define		_MAIN_H_

	u8 findPath		( u32 start, u32 end );
	u8 setObstacle		( Obstacle obstacle, Type type, u16 ticks );
	u8 unsetObstacle	( Obstacle obstacle, Type type );
	u8 update		( u32 ticks );
	u8 nextStep		( u32 *coordinates );

	Map map			= MAP;
	Map mapSave		= MAP;
	
	// Permet d'afficher la carte sans les obstacles ajoutés à cause de la taille du robot (à ne pas embarquer) :
	Map mapBis		= MAPBIS;

	u16  step		= 0;

	Path path		= { 0 };
	u16  pathLength		= 0;

	Path pathSmoothed	= { 0 };
	u16  pathSmoothedLength = 0;

	Robot robot		= { 0, { 0, 0 } };
	PawnList pawnList	= { { { 0, 0 } }, { 0 }, FALSE };
	
	Heap open_list		= { { { 0, 0, 0, 0, 0, NULL } }, 0 };
	Heap closed_list	= { { { 0, 0, 0, 0, 0, NULL } }, 0 };

#endif

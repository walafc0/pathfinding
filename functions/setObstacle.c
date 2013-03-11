#include	"shared.h"
#include	"setObstacle.h"

extern PawnList pawnList;
extern Robot robot;
extern Map map;

u8 setObstacle ( Obstacle obstacle, Type type, u16 ticks )
{
	obstacle.firstCoordinates = COORD((X(obstacle.firstCoordinates)-ROBOT_SIZE), (Y(obstacle.firstCoordinates)-ROBOT_SIZE)) ;
	obstacle.lastCoordinates = COORD((X(obstacle.lastCoordinates)+ROBOT_SIZE), (Y(obstacle.lastCoordinates)+ROBOT_SIZE)) ;

	if ( type == PAWN )
	{
		Obstacle pawn = obstacle ;
		u8 index = MAX_PAWN ;

		for ( u8 i = 0; i < MAX_PAWN; i++ )
		{
			if ( !pawnList.index[i] )
			{
				index = i ;
				break ;
			}
		}

		if ( index == MAX_PAWN )
			return ERR_1 ;

		pawnList.index[index] = BUSY ;
		pawnList.obstacle[index] = pawn ;
	}
	else
	{
		Obstacle obs = obstacle ;		// Valeur qui ne sera pas utilisé dans unsetObstacle()...
		unsetObstacle ( obs, ROBOT );

		robot.ticks = ticks;
		robot.obstacle.firstCoordinates = obstacle.firstCoordinates;
		robot.obstacle.lastCoordinates = obstacle.lastCoordinates;
	}
	u16 x1 = X(obstacle.firstCoordinates) ;
	u16 y1 = Y(obstacle.firstCoordinates) ;
	u16 x2 = X(obstacle.lastCoordinates) ;
	u16 y2 = Y(obstacle.lastCoordinates) ;

	for ( u16 i = y1; i <= y2; i++ )
		for ( u16 j = x1; j <= x2; j++ )
			map[i][j/4] |= 0x03 << ( 6 - ( ( j % 4) * 2 ) ) ;

	return SUCCESS;
}

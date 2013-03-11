#include	"shared.h"
#include	"unsetObstacle.h"

extern PawnList pawnList;
extern Robot robot;
extern Map map;
extern Map mapSave;

u8 unsetObstacle ( Obstacle obstacle, Type type )
{
	if ( type == PAWN )
	{
		obstacle.firstCoordinates = COORD((X(obstacle.firstCoordinates)-ROBOT_SIZE), (Y(obstacle.firstCoordinates)-ROBOT_SIZE)) ;
		obstacle.lastCoordinates = COORD((X(obstacle.lastCoordinates)+ROBOT_SIZE), (Y(obstacle.lastCoordinates)+ROBOT_SIZE)) ;

		u8 index = MAX_PAWN ;
		for ( u8 i = 0; i < MAX_PAWN; i++ )
		{
			if ( pawnList.index[i] )
			{
				if ( pawnList.obstacle[i].firstCoordinates == obstacle.firstCoordinates
					  && pawnList.obstacle[i].lastCoordinates == obstacle.lastCoordinates )
				{
					pawnList.index[i] = !BUSY;
					index = i;
					break;
				}
			}
		}

		if ( index == MAX_PAWN )
			return FAILURE ;
	}
	else
	{


		obstacle.firstCoordinates = COORD((X(robot.obstacle.firstCoordinates)-ROBOT_SIZE), (Y(robot.obstacle.firstCoordinates)-ROBOT_SIZE)) ;
		obstacle.lastCoordinates = COORD((X(robot.obstacle.lastCoordinates)+ROBOT_SIZE), (Y(robot.obstacle.lastCoordinates)+ROBOT_SIZE)) ;

		robot.ticks = 0;
		robot.obstacle.firstCoordinates = 0;
		robot.obstacle.lastCoordinates = 0;
	}

	u16 x1 = X(obstacle.firstCoordinates) ;
	u16 y1 = Y(obstacle.firstCoordinates) ;
	u16 x2 = X(obstacle.lastCoordinates) ;
	u16 y2 = Y(obstacle.lastCoordinates) ;

	u8 currentValue ;
	u8 maskForCurrentValue ;

	u8 originalValue ;
	u8 maskForOriginalValue ;

	for ( u16 i = y1; i <= y2; i++ )
	{
		for ( u16 j = x1; j <= x2; j++ )
		{
			currentValue = map[i][j/4] ;
			maskForCurrentValue = 0xff << ( 8 - (j%4)*2 ) | 0xff >> ( 2 + (j%4)*2 ) ;

			originalValue = mapSave[i][j/4];
			maskForOriginalValue = 0x03 << ( 6 - (j%4)*2 ) ;

			map[i][j/4] = ( currentValue & maskForCurrentValue ) | ( originalValue  & maskForOriginalValue ) ;

			// En plus court :
			// map[i][j/4] = ( map[i][j/4] & ( 0xff << ( 8 - (j%4)*2 ) | 0xff >> ( 2 + (j%4)*2 ) ) ) | ( mapSave[i][j/4]  & ( 0x03 << ( 6 - (j%4)*2 ) ) ) ;
			// ;^D
		}
	}


	return SUCCESS;
}

#include	"shared.h"
#include	"update.h"

extern Robot robot;

u8 update ( u16 ticks )
{


	if ( robot.ticks + TICKS_TO_UPDATE < ticks )
	{
		Obstacle obstacle = { 0x00000000, 0x00000000 } ;	// Pour pas mettre NULL, mais initule...
		unsetObstacle ( obstacle, ROBOT ) ;
		return SUCCESS ;
	}
	return FAILURE ;
}

#include	"shared.h"
#include	"nextStep.h"

extern u16 step ;
extern u16 pathSmoothedLength ;
extern Path pathSmoothed ;

u8 nextStep ( u32 *coordinates )
{
	if ( step >= pathSmoothedLength )
	{
		step = 0 ;
		return FAILURE ;
	}

	*coordinates = pathSmoothed[step++] ;

	return SUCCESS ;
}

#include	<stdio.h>

#include	"shared.h"
#include	"main.h"
#include	"affichage.h"

int main ( int argc, char *argv[] )
{
	u8 error;
	u32 start;
	u32 end;
	u32 coordinates;

	/* An example of starting and ending points */
	start = COORD ( 10, 10 );
	end = COORD ( 250, 150 );

	error = findPath ( start, end );
	if ( error )
	{
		printf ( "Erreur %d\n", error );
		return FAILURE;
	}

	error = nextStep ( &coordinates );
	while ( !error )
	{
		error = nextStep ( &coordinates );
		printf ( "Prochain point : x = %d ; y = %d\n", (int) X ( coordinates ), (int) Y ( coordinates ) );
	}

	affichage() ;
	return SUCCESS ;
}

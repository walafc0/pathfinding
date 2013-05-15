#ifndef _SHARED_H_
	#define		_SHARED_H_

	#include	"config.h"
	#include	"errno.h"

	#ifndef	NULL
		#define NULL	0
	#endif

	#ifndef	TRUE
		#define TRUE	1
	#endif
	
	#ifndef	FALSE
		#define	FALSE	0
	#endif
	
	#define	BUSY		3
	#define	PATH_MAX_LENGTH	MAX_LOOP	// MAX_LOOP défini dans `config.h`
	
	#define	MAX_LIST	(( MAP_HEIGHT * MAP_WIDTH ) / 4)	// Arbitraire : 1/4 du terrain

	#define	X(n)		( (n) >> 16 )
	#define	Y(n)		( (n) & 0xffff )
	#define	COST(x, y)	( map[(y)][(x)/4] >> ( 6 - ( ( (x) % 4 ) * 2 ) ) & 0x03 )
	#define	COORD(x, y)	( ( (x) << 16 ) + (y) )

	#ifndef TPL_OS_STD_TYPES_GENERIC_H
		typedef	signed char		s8 ;				// De -128 à 127
		typedef	unsigned char 		u8 ;				// De 0 à 255
		
		typedef	signed short int	s16 ;				// De -32 768 à 32 767
		typedef	unsigned short int	u16 ;				// De 0 à 65 535
		
		typedef	unsigned long int	u32 ;				// De 0 à 4 294 967 295
	#endif	
		
	typedef	u32			Path [PATH_MAX_LENGTH] ;
	typedef	u8			Map  [MAP_HEIGHT][MAP_WIDTH] ;

	typedef struct Obstacle Obstacle;
	struct Obstacle
	{
		u32	firstCoordinates;
		u32	lastCoordinates;
	};

	typedef struct Robot Robot;
	struct Robot
	{
		u16	ticks;
		Obstacle obstacle;
	};

	typedef struct PawnList PawnList;
	struct PawnList
	{
		Obstacle obstacle[MAX_PAWN];
		u8 index[MAX_PAWN];
		u8 isInitialized;
	};
	
	typedef struct Node Node ;
	struct __attribute__((__packed__)) Node
	{
		u8 open_list ;
		u8 closed_list ;
		u16 cost_g ;
		u16 cost_h;
		u32 coordinates ;
		Node *parent ;
	} ;

	typedef struct Heap Heap;
	struct Heap
	{
		Node node[MAX_LIST];
		u16 count ;
	} ;

	typedef enum Type { ROBOT, PAWN } Type;

#endif

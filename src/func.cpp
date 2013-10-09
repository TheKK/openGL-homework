
#include "func.h"

void rotateX( GLdouble vertex[][ 3 ], double degree )
{
	double container[ 3 ];
	double result[ 3 ];
	double temp;
	double rotateMetrix[][ 3 ] ={
	{ 1,            0,                  0 },
	{ 0, cos( degree ),-1 * sin( degree ) },
	{ 0, sin( degree ),     cos( degree ) } };
	
	for( int i = 0; i < 8; i++ ){
		for( int x = 0; x < 3; x++ )	container[ x ] = vertex[ i ][ x ];

		for( int row = 0; row < 3; row++ ){	temp = 0;
			for( int col = 0; col < 3; col++){	
				temp += rotateMetrix[ row ][ col ] * container[ col ];
			}result[ row ] = temp;
		}
		
		for( int y = 0; y < 3; y++ )	vertex[ i ][ y ] = result[ y ];
	}		
}

void rotateY( GLdouble vertex[][ 3 ], double degree )
{
	double container[ 3 ];
	double result[ 3 ];
	double temp;
	double rotateMetrix[][ 3 ] ={
	{ cos( degree ), 0,-1 * sin( degree ) },
	{             0, 1,                 0 },
	{ sin( degree ), 0,     cos( degree ) } };
	
	for( int i = 0; i < 8; i++ ){
		for( int x = 0; x < 3; x++ )	container[ x ] = vertex[ i ][ x ];

		for( int row = 0; row < 3; row++ ){	temp = 0;
			for( int col = 0; col < 3; col++){	
				temp += rotateMetrix[ row ][ col ] * container[ col ];
			}result[ row ] = temp;
		}
		
		for( int y = 0; y < 3; y++ )	vertex[ i ][ y ] = result[ y ];
	}		
}

void rotateZ( GLdouble vertex[][ 3 ], double degree )
{
	double container[ 3 ];
	double result[ 3 ];
	double temp;
	double rotateMetrix[][ 3 ] ={
	{ cos( degree ),-1 * sin( degree ), 0 },
	{ sin( degree ),     cos( degree ), 0 },
	{             0,                 0, 1 } };
	
	for( int i = 0; i < 8; i++ ){
		for( int x = 0; x < 3; x++ )	container[ x ] = vertex[ i ][ x ];

		for( int row = 0; row < 3; row++ ){	temp = 0;
			for( int col = 0; col < 3; col++){	
				temp += rotateMetrix[ row ][ col ] * container[ col ];
			}result[ row ] = temp;
		}
		
		for( int y = 0; y < 3; y++ )	vertex[ i ][ y ] = result[ y ];
	}		
}

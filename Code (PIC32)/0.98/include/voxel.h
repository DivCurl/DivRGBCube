#ifndef VOXEL_H
#define	VOXEL_H

#include "main.h"
#include "driver.h"

#ifdef cplusplus
extern "C" {
#endif

void V_Set( int x, int y, int z, rgb_t color );
void V_Clr( int x, int y, int z );
bool V_Fly( const char *plane, int16 str, int16 end, int16 offs_ortho, int16 offs_perp );
void v_Swap( uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2 );
void V_Flip( uint8 x, uint8 y, uint8 z, rgb_t color );
bool V_State( uint8 x, uint8 y, uint8 z );
rgb_t V_GetRGB( uint8 x, uint8 y, uint8 z );

#ifdef	__cplusplus
}
#endif

#endif	


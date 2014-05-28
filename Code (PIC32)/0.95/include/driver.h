#ifndef DRIVER_H
#define	DRIVER_H

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

void D_ClrFrame(void);
void D_FBUpdate(void);
void D_Line(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 z1, uint8 z2, rgb_t rgb);
void D_FillPlaneX(uint8 X, rgb_t color);
void D_ClrPlaneX(uint8 X);
void D_FillPlaneY(uint8 Y, rgb_t color);
void D_ClrPlaneY(uint8 Y);
void D_FillPlaneZ(uint8 Z, rgb_t color);
void D_ClrPlaneZ(uint8 Z);
void D_FillCol(int x, int y, int height, rgb_t rgb);
void D_ClrCol(int x, int y);
void D_Shift(char axis, const int);
bool D_Bounds(int x, int y, int z);  // params signed int to allow checks on negative input values
bool D_GetFlag(uint16 *FLAGS, const uint16 flag_val);
bool D_Delayms(uint32 delay);
int32 D_Round(float input);
float D_Remap(float in, float inMin, float inMax, float outMin, float outMax);
float D_Abs(float input);
struct animNode_t* D_ListSearchAnim( anim_t anim, struct animNode_t **prev );
struct animNode_t* D_ListAddAnim( anim_t anim, bool addEnd );
struct animNode_t* D_ListCreate( anim_t anim );
int D_ListDeleteAnim( anim_t anim );

#ifdef	__cplusplus
}
#endif

#endif	


#ifndef VOXEL_H
#define	VOXEL_H

#include "main.h"
#include  "driver.h"

#ifdef cplusplus
extern "C" {
#endif

void Voxel_Set_Solid_RGB(uint8 x, uint8 y, uint8 z, SOLID_RGB color);
void Voxel_Set_RGB(uint8 x, uint8 y, uint8 z, RGB color);
RGB Voxel_Get_RGB(uint8 x, uint8 y, uint8 z);
void Voxel_Clr(uint8 x, uint8 y, uint8 z);

#ifdef	__cplusplus
}
#endif

#endif	


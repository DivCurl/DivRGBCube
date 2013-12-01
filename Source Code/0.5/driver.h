#ifndef DRIVER_H
#define	DRIVER_H

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define myPI  3.14159265358979323846
#define myDPI 1.2732395
#define myDPI2 0.40528473

float Abs(float input);
int16 Round(float input);
float Remap(float in, float inMin, float inMax, float outMin, float outMax);
void VoxelSetSolidRGB(uint8 x, uint8 y, uint8 z, SOLID_RGB color);
void VoxelSetRGB(uint8 x, uint8 y, uint8 z, RGB color);
void VoxelClr(uint8 x, uint8 y, uint8 z);
void ClrCube(void);
float mySin(float x);
float myCos(float x);
float mySqrt(float in);

#ifdef	__cplusplus
}
#endif

#endif	


#ifndef DRIVER_H
#define	DRIVER_H

#include "main.h"
#include "types.h"
#include "rgb.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    IN,
    OUT
} SHIFT_DIR;

float Abs(float input);
int16 Round(float input);
float Remap(float in, float inMin, float inMax, float outMin, float outMax);
void Clear_Frame(void);
void FB_Update(void);
uint8 Delay_mS(uint32 delay);
void Fill_Plane_X(uint8 X);
void Clear_Plane_X(uint8 X);
void Fill_Plane_Y(uint8 Y);
void Clear_Plane_Y(uint8 Y);
void Fill_Plane_Z(uint8 Z);
void Clear_Plane_Z(uint8 Z);
void Shift(char axis, SHIFT_DIR dir);


#ifdef	__cplusplus
}
#endif

#endif	


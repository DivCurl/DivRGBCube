#ifndef RGB_H
#define	RGB_H

#include "main.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

RGB g_cube_prebaked[8][8][8];
RGB Get_Color_RGB(uint8, uint8, uint8);
void Fade_CW_Cont();
void Prebake_Cube_Colors();
void Fade_Prebaked();
    
#ifdef	__cplusplus
}
#endif

#endif


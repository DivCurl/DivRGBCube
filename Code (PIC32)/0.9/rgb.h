#ifndef RGB_H
    #define	RGB_H

    #include "main.h"
    #include "types.h"

    #ifdef	__cplusplus
    extern "C" {
    #endif

    RGB g_cube_prebaked[8][8][8];
    RGB RGBGetColor(uint8, uint8, uint8);
    void RGBRefreshColorBuffer(void);
    void RGBFadeCWCont(void);
    void RGBFadeSolid(void);
    void RGBPrebakeColors(void);
    void RGBFadePrebaked(void);
    void RGBFadeWipe(void);

    #ifdef	__cplusplus
    }
    #endif

#endif


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

    float DRVAbs(float input);
    int16 DRVRound(float input);
    float DRVRemap(float in, float inMin, float inMax, float outMin, float outMax);
    void DRVClrFrame(void);
    void DRVFBUpdate(void);
    uint8 DRVDelayms(uint32 delay);
    void DRVLine(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 z1, uint8 z2);
    void DRVFillPlaneX(uint8 X);
    void DRVClrPlaneX(uint8 X);
    void DRVFillPlaneY(uint8 Y);
    void DRVClrPlaneY(uint8 Y);
    void DRVFillPlaneZ(uint8 Z);
    void DRVClrPlaneZ(uint8 Z);
    void DRVShift(char axis, SHIFT_DIR dir);
    bool DRVBounds(int32 x, int32 y, int32 z);  // params signed int to allow checks on negative input values

    #ifdef	__cplusplus
    }
    #endif

#endif	


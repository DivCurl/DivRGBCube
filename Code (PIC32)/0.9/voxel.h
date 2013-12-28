#ifndef VOXEL_H
    #define	VOXEL_H

    #include "main.h"
    #include  "driver.h"

    #ifdef cplusplus
    extern "C" {
    #endif

    void VXSet(uint8 x, uint8 y, uint8 z, RGB color);
    void VXSetSolid(uint8 x, uint8 y, uint8 z, SOLID_RGB color);
    void VXClr(uint8 x, uint8 y, uint8 z);
    RGB VXGetRGB(uint8 x, uint8 y, uint8 z);
    

    #ifdef	__cplusplus
    }
    #endif

#endif	


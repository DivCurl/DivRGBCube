#ifndef ANIMS_H
    #define	ANIMS_H

    #include "main.h"
    #include "rgb.h"
    #include "engine.h"

    #ifdef cplusplus
    extern "C" {
    #endif

    // Prototypes
    void Animate(uint8, uint16);
    void ANCubeTest(void);
    void ANSineWave2D(void);
    void ANSineWave3D(void);
    void ANRain(void);
    void ANCubeFillDazzle();
    void ANPathMove (unsigned char *, int);
    void ANSendText(const char* );
    void ANTranslate2D(void);
    void ANRotate2D(void);
    void ANRotate3D(void);

    #ifdef	__cplusplus
    }
    #endif

#endif	


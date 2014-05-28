#ifndef RGB_H
#define	RGB_H

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define FADE_CW_CONT        1
#define FADE_CCW_CONT       2
#define FADE_CW_INPUT       3
#define FADE_PREBAKED       4
#define FADE_SOLID          5
#define FADE_WIPE           6
#define COLOR_ARRAY_LEN     1536

// Some common solid color defs
#define RGB_RED             (255>>CUBE_BCM_RES),0,0
#define RGB_ORANGE          (255>>CUBE_BCM_RES),(80>>CUBE_BCM_RES),0
#define RGB_YELLOW          (255>>CUBE_BCM_RES),(255>>CUBE_BCM_RES),0
#define RGB_GREEN           0,(255>>CUBE_BCM_RES),0
#define RGB_CYAN            0,(255>>CUBE_BCM_RES),(255>>CUBE_BCM_RES)
#define RGB_BLUE            0,0,(255>>CUBE_BCM_RES)
#define RGB_VIOLET          (60>>CUBE_BCM_RES),0,(255>>CUBE_BCM_RES)
#define RGB_MAGENTA         (255>>CUBE_BCM_RES),0,(255>>CUBE_BCM_RES)

rgb_t R_GetColorCoord( uint8, uint8, uint8 );
rgb_t R_GetAngleColor( float theta );
rgb_t R_SetColorRGB( int r, int g, int b );
rgb_t R_RandomColor();
void R_RefreshColorBuffer( uint8 mode );
void R_FadeCWCont( void );
void R_FadeSolid( void );
void R_FadeWipe( void );
void R_RandomizeModes( void );
void R_GetRandomDelay( void );
void R_GetRandomFadeDelay( void );
void R_GetRandomFadeROC( void );
void R_GetRandomFadeMode( void );
void R_CubeSetSolid (rgb_t color );

#ifdef	__cplusplus
}
#endif

#endif


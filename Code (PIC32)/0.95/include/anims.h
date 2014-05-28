#ifndef ANIMS_H
#define	ANIMS_H

#include "main.h"
#include "matrix.h"
#include "vector.h"

#ifdef cplusplus
extern "C" {
#endif

void A_Animate( int anim );
void A_CubeTest( void );
void A_CubeSplash(void);
void A_SineWave2D( void );
void A_SineWave3D( void );
void A_Rain( void );
void A_CubeFillDazzle();
void A_PathBitmap( char bitmap );
void A_PathMove( unsigned char *, int );
void A_CircleRotate( uint8 radius, uint8 c_x, uint8 c_y, uint8 c_z );
void A_Translate2D( void );
void A_Rotate2D( void );
void A_Rotate3D( void );
void A_VUMeter( void );
void A_SpecAnalyzer( void );
void A_CrossHatch( void );
void A_FacePaletteSwap( rgb_t color1, rgb_t color2 );
void A_Helix();
void A_DoubleHelix();
void A_DNA();

#ifdef	__cplusplus
}
#endif

#endif	


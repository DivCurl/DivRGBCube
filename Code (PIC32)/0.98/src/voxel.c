#include "../include/voxel.h"

/*
===================
 Name

 Desc
===================
*/
void V_Set(int x, int y, int z, rgb_t color) {

    if ( !D_Bounds( x, y, z ) ) {
        return;
    }

    if ( TSTFLAG ( xsig.AFLAGS, ANIM_USE_GLOBAL ) ) {
        if ( xsig.rgbMode == RGB_MODE_SOLID )  {
            g_colorBuffer[x][y][z] = xsig.color;
        }

        if ( xsig.rgbMode == RGB_MODE_FADE ) {
            g_colorBuffer[x][y][z] = R_GetAngleColor( ptrAnim->rgbOpts.angle );
        }

        if ( xsig.rgbMode == RGB_MODE_DEF ) {
            g_colorBuffer[x][y][z] = color;
        }
    } else {
   //     if ( color.r != 0 || color.g != 0 && color.b != 0 ) {
            g_colorBuffer[x][y][z] = color;
  //      } else {
  //          g_colorBuffer[x][y][z] = R_GetAngleColor( ptrAnim->rgbOpts.angle );
    //    }
    }
}

/*
===================
 Name

 Desc
===================
*/
void V_Clr( int x, int y, int z ) {
    // if fadeout flag is set, shift values of this voxel right once to
    // achieve a fading effect



    if ( TSTFLAG( ptrAnim->rgbOpts.FLAGS, RGB_VOX_OFF_FADEOUT )
            || TSTFLAG( xsig.RFLAGS, RGB_VOX_OFF_FADEOUT) ) {
        g_colorBuffer[x][y][z].r >>= 1;
        g_colorBuffer[x][y][z].g >>= 1;
        g_colorBuffer[x][y][z].b >>= 1;
        // otherwise just fully turn it off
    } else {
        g_colorBuffer[x][y][z].r = 0;
        g_colorBuffer[x][y][z].g = 0;
        g_colorBuffer[x][y][z].b = 0;
    }
}

/*
===================
 Name

 Desc
===================
*/
bool V_Fly( const char *plane, int16 start, int16 end, int16 offs_ortho, int16 offs_perp ) {
   //  static uint8 ctr;
    int16 i;

    if ( !strcmp( plane, "XY" ) ) {
        for ( i = start; i <= end; i++ ) {
            if ( TSTFLAG ( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
                return;
            }
            V_Set( offs_ortho, i, offs_perp, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
            if ( ( i-1 )  >= 0 ) // we don't want to go outside the buffer
                    V_Clr( offs_ortho, ( i - 1 ), offs_perp );   // Clear the previous Voxel before updating the current one
            D_FBUpdate();
            while ( !D_Delayms( (float)1/ptrAnim->rate * 30000.f ) ) {}   // short delay to allow each voxel in sequence to be shown before next one is lit
        }
    }

    if ( !strcmp( plane, "XZ" ) ) {


    }

    if ( !strcmp( plane, "YZ") ) {

        
    }
 }

/*
===================
 Name

 Desc
===================
*/
bool V_State(uint8 x, uint8 y, uint8 z) {
    if (g_colorBuffer[x][y][z].r == 0
            && g_colorBuffer[x][y][z].g == 0
            && g_colorBuffer[x][y][z].b == 0)
        return 0;
    else
        return 1;
}

/*
===================
 Name

 Desc
===================
*/
void V_Swap(uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2) {
    rgb_t tmp1, tmp2;
    tmp1 = g_colorBuffer[x1][y1][z1];
    tmp2 = g_colorBuffer[x2][y2][z2];
    g_colorBuffer[x1][y1][z1] = tmp2;
    g_colorBuffer[x2][y2][z2] = tmp1;
}

/*
===================
 Name

 Desc
===================
*/
void V_Mov( uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2 ) {
    g_colorBuffer[x2][y2][z2] = g_colorBuffer[x1][y1][z1];
    V_Clr( x1, y1, z1 );
}

/*
===================
 V_Flip

 Takes a voxel x,y,z coordinate as input and flips the current state of the
 voxel.
===================
*/
void V_Flip( uint8 x, uint8 y, uint8 z, rgb_t color ) {
    if ( !V_State( x, y, z ) ) {
        V_Set(x, y, z, color);
    } else {
        V_Clr( x, y, z );
    }
}
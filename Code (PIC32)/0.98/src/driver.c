#include "../include/driver.h"

rgb_t g_frameBuffer[8][8][8];
extern unsigned long g_msCounter;

/*
===================
 Name

 Desc
===================
*/
void D_TestUART(void){
    char filename[50] ;                         //Array of 50 chars
    memset(filename, 0, 50*sizeof(char));         //Clears the Array
    sprintf(filename, "Testing");               //Places the String into the Array
    putsUART1(filename);                        //Sends the Array over UART1

    int variable=0;
    int index=0;

    while (1) {
        sprintf(filename, "%d\n",variable); //%d will get the value of the next argument in the function and place it in the string
        putsUART1(filename);                //Sends the Array over UART1
        variable++;                         //Increments the variable by 1

        for(index=0; index < 4000000; index++) //a dummy function ot delay the loop
            Nop(); // no operation for this cycle
    }
}

/*
===================
 Name

 Desc
===================
*/
long D_SetCtrmS(float in_ms) {
    // T4 interrupt is 1 tick/50uS -> 20 ticks / 1 mS
    return in_ms * 20;
}

/*
===================
 Name

 Desc
===================
*/
bool D_Delayms( float ms ) {
    long delay;
    static long count, tmp = 0;

    delay = ms;// * 20;

    if (g_msCounter != tmp) {
        tmp = g_msCounter;
        count++;
    }

    if (count >= delay) {
        count = 0;
        tmp = 0;
        return 1;
    }
    
    else
        return 0;

}

/*
===================
 D_Dist2D

 Returns euclidian distance between two points on 2D plane surface
===================
*/
float D_Dist2D( float x1,  float y1, float x2, float y2 ) {
  float dist;
  dist = sqrt( ( x1-x2 )*( x1-x2 ) + ( y1-y2 )*( y1-y2 ) );

  return dist;
}

/*
===================
 D_Dist3D

 Returns euclidian distance between two points within cube
===================
*/
D_Dist3D( float x1,  float y1, float z1, float x2, float y2, float z2 ) {
  float dist;
  dist = sqrt( ( x1-x2 )*( x1-x2 ) + ( y1-y2 )*( y1-y2 ) + ( z1-z2 )*( z1-z2 ) );

  return dist;
}



/*
===================
 D_Abs

 Returns absolute value of input
===================
*/
float D_Abs( float in ) {
    return (in > 0 ? in : -in);
}

/*
===================
 Name

 Desc
===================
*/
int32 D_Round( float in ) {
    int16 s = in / D_Abs( in );
    return (int16)(s * ( D_Abs( in ) + 0.5 ) );
}

/*
===================
 Name

 Desc
===================
*/
bool D_Bounds( int x, int y, int z ) {
    if (x >= 0 && x <= CUBE_X_MAX &&
        y >= 0 && y <= CUBE_Y_MAX &&
        z >= 0 && z <= CUBE_Z_MAX)
        return TRUE;    
    else
        return FALSE;
}

/*
===================
 Name

 Desc
===================
*/
float D_Remap( float in, float inMin, float inMax, float outMin, float outMax ) {
	float out;
	out = ( in - inMin ) / ( inMax - inMin ) * ( outMax - outMin ) + outMin;
	return out;
}

/*
===================
 Name

 Desc
===================
*/
struct animNode_t* D_ListCreate( anim_t anim ) {
    struct animNode_t *ptr = ( struct animNode_t* ) malloc ( sizeof(struct animNode_t) );
    if ( ptr == NULL ) {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->anim = anim;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

/*
===================
 Name

 Desc
===================
*/
struct animNode_t* D_ListAddAnim( anim_t anim, bool addEnd ) {

}

/*
===================
 Name

 Desc
===================
*/
int D_ListDeleteAnim( anim_t anim ) {
    struct animNode_t *prev = NULL;
    struct animNode_t *del = NULL;

    del = D_ListSearchAnim( anim, &prev );
    if( del == NULL ) {
        return -1;
    } else {
        if(prev != NULL) {
            prev->next = del->next;
        }
        if(del == curr) {
            curr = prev;
        }   else if( del == head ) {
            head = del->next;
        }
    }

    free( del );
    del = NULL;

    return 0;
}

/*
===================
 Name

 Desc
===================
*/
struct animNode_t* D_ListSearchAnim( anim_t anim, struct animNode_t **prev ) {

}

/*
===================
 Name

 Desc
===================
*/
void D_Line( uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2, rgb_t rgb ) {
    int i, dx, dy, dz, l, m, n;
    int x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    uint8 point[3];

    point[0] = x1;
    point[1] = y1;
    point[2] = z1;
    dx = x2 - x1;
    dy = y2 - y1;
    dz = z2 - z1;
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;

    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        
        for (i = 0; i < l; i++) {
            V_Set(point[0], point[1], point[2], rgb);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point[0] += x_inc;
        }
    }
    else if ((m >= 1) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;

        for (i = 0; i < m; i++) {
            V_Set(point[0], point[1], point[2], rgb);
            if (err_1 > 0) {
                point[0] += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point[2] += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point[1] += y_inc;
        }
    }
    else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        
        for (i = 0; i < n; i++) {
            V_Set(point[0], point[1], point[2], rgb);
            if (err_1 > 0) {
                point[1] += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point[0] += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point[2] += z_inc;
        }
    }
    
    V_Set(point[0], point[1], point[2], rgb);
}

/*
===================
 Name

 Desc
===================
*/
void D_Rect( uint8* center_pt, const char *plane, uint8 vox_l, uint8 vox_w ) {
    if (!(strcmp(plane, "XY"))) {
    }

    if (!(strcmp(plane, "XZ"))) {
    }

    if (!(strcmp(plane, "YZ"))) {
    }

}

/*
===================
 Name

 Desc
===================
*/
void D_Cube( void ) {
// Idea: draw rectangle and stretch it by translation?

}

/*
===================
 Name

 Desc
===================
*/
void D_Circle( uint8 rad, uint8 xc, uint8 yc, uint8 cz ) {
    // float x, y, z;
    uint8 x, y, z;
    float t;

    for (t = 0; t <= TWO_PI; t += 0.1) {
        x = D_Round(rad * cos(t));
        x = x + xc;

        y = D_Round(rad * sin(t));
        y = y + yc;

   //     vxSet(x, y, 3, rgbGetColorCoord(x, y, 3));
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_FillPlaneX( uint8 X, rgb_t color ) {
    uint8 y, z;
    if (X >= 0 && X <= CUBE_X_MAX) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                V_Set(X, y, z, color);
            }
        }
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_ClrPlaneX( uint8 X ) {
    uint8 y, z;
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                V_Clr(X, y, z);
            }
        }
 }

/*
===================
 Name

 Desc
===================
*/
void D_FillPlaneY( uint8 Y, rgb_t color ) {
    uint8 x, z;
    if (Y >= 0 && Y <= CUBE_Y_MAX) {
        for (x = 0; x <= CUBE_X_MAX; x++) {
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                V_Set(x, Y, z, color);
            }
        }
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_ClrPlaneY( uint8 Y )  {
    uint8 x, z;

    for (x = 0; x <= CUBE_X_MAX; x++) {
        for (z = 0; z <= CUBE_Z_MAX; z++) {
            V_Clr(x, Y, z);
        }
    }
 }

/*
===================
 Name

 Desc
===================
*/
void D_FillPlaneZ( uint8 Z, rgb_t color ) {
    uint8 x, y;

    if (Z >= 0 && Z <= CUBE_Z_MAX) {
        for (x = 0; x <= CUBE_X_MAX; x++) {
            for (y = 0; y <= CUBE_Y_MAX; y++) {
                V_Set(x, y, Z, color);
            }
        }
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_ClrPlaneZ(uint8 Z) {
    uint8 x, y;

    for (x = 0; x <= CUBE_X_MAX; x++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            V_Clr(x, y, Z);
        }
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_FillCol(int x, int y, int height, rgb_t color) {
    int z;
    for (z = 0; z <= CUBE_Z_MAX; z++) {
        if ((z+1) <= height)
            V_Set(x, y, z, color);
        else 
            V_Clr(x, y, z);
    }
}

/*
===================
 Name

 Desc
===================
*/
void D_ClrCol( int x, int y ) {
    int z;
    for (z = 0; z <= CUBE_Z_MAX; z++)
        V_Clr(x, y, z);
}

/*
===================
 Name

 Desc
===================
*/
void D_Shift( char axis, const int dir ) {
    uint8 x, y, z;
    rgb_t rgb[8][8][8];
    // load temp buffer with current frame of cube output buffer
    memcpy( rgb, g_frameBuffer, sizeof( g_frameBuffer ) );

    switch (axis) {
       case ('X') :
            if (dir == SHIFT_RIGHT ) {
                for (x = 0; x < CUBE_X_MAX; x++) {
                    for (y = 0; y <= CUBE_Y_MAX; y++) {
                        for (z = 0; z <= CUBE_Z_MAX; z++) {
                            g_colorBuffer[x+1][y][z] = rgb[x][y][z];

                        }
                    }
                }
                D_ClrPlaneX(0);
            }

            if (dir == SHIFT_LEFT ) {
                for (x = CUBE_X_MAX; x > 0; x--) {
                    for (y = 0; y <= CUBE_Y_MAX; y++) {
                        for (z = 0; z <= CUBE_Z_MAX; z++) {
                            g_colorBuffer[x-1][y][z] = rgb[x][y][z];

                        }
                    }
                }
                D_ClrPlaneX(CUBE_X_MAX);
            }
            break;      // Case 'X'

        case ('Y') :
            if (dir == SHIFT_OUT ) {
                for (y = 0; y < CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        for (z = 0; z <= CUBE_Z_MAX; z++) {
                            g_colorBuffer[x][y+1][z] = rgb[x][y][z];
                        }
                    }
                }
                D_ClrPlaneY(0);
            }

            if (dir == SHIFT_IN ) {
                for (y = CUBE_Y_MAX; y > 0; y--) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        for (z = 0; z <= CUBE_Z_MAX; z++) {
                            g_colorBuffer[x][y-1][z] = rgb[x][y][z];

                        }
                    }
                }
                D_ClrPlaneY( CUBE_Y_MAX );
            }
            break;      // Case 'Y'

       case ('Z') :
            if ( dir == SHIFT_UP ) {
                for ( z = 0; z < CUBE_Z_MAX; z++ ) {
                    for ( y = 0; y <= CUBE_Y_MAX; y++ ) {
                        for ( x = 0; x <= CUBE_X_MAX; x++ ) {
                            g_colorBuffer[x][y][z+1] = rgb[x][y][z];

                        }
                    }
                }
                D_ClrPlaneZ(0);
            }

            if (dir == SHIFT_DOWN ) {
                for (z = CUBE_Z_MAX; z > 0; z--) {
                    for (y = 0; y <= CUBE_Y_MAX; y++) {
                        for (x = 0; x <= CUBE_X_MAX; x++) {
                            g_colorBuffer[x][y][z-1] = rgb[x][y][z];
                        }
                    }
                }    
                D_ClrPlaneZ(CUBE_Z_MAX);
            }
            break;      // Case 'Z'
            
        default:
            break;
    } // switch
}

/*
===================
 Name

 Desc
===================
*/
void D_ClrFrame() {
    // if fadeout flag is set, any voxel with a color value will get gradually faded.
    // I wonder if there is a way to do this without a 512-step loop?
    if ( TSTFLAG( ptrAnim->rgbOpts.FLAGS, RGB_VOX_OFF_FADEOUT )
            || TSTFLAG( xsig.RFLAGS, RGB_VOX_OFF_FADEOUT ) ) {
        uint8 x, y, z;
        for ( x = 0; x <= CUBE_X_MAX; x++ ) {
            for ( y = 0; y <= CUBE_Y_MAX; y++ ) {
                for ( z = 0; z <= CUBE_Z_MAX; z++ ) {
                    g_colorBuffer[x][y][z].r >>= 1;
                    g_colorBuffer[x][y][z].g >>= 1;
                    g_colorBuffer[x][y][z].b >>= 1;
                }
            }
        }
    // otherwise just set all the voxels to off in the buffer
    } else {
        memset( g_colorBuffer, 0, sizeof( g_colorBuffer ) );
    }
 }

/*
===================
 Name

 Desc
===================
*/
void D_FBUpdate() {
    // Note: disable T2 refresh interrupts while in this loop?
 //   mT2IntEnable(0);
    uint8 x, y, z;
    for (z = 0; z <= CUBE_Z_MAX; z++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (x = 0; x <= CUBE_X_MAX; x++) {
                    // using color_negative, we can specify a 'blank' color other
                    // than OFF (0) when refreshing               
                if (!V_State( x, y, z ) || R_ColorEqual( g_colorBuffer[x][y][z], ptrAnim->rgbOpts.rgbMask ) ) {
                    g_colorBuffer[x][y][z] = ptrAnim->rgbOpts.rgbMask;
                }
                
                // Update framebuffer voxel value
                g_frameBuffer[x][y][z] = g_colorBuffer[x][y][z];
            }
        }
    }
//    mT2IntEnable(1);
}    
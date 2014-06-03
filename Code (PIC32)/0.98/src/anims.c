/******************************************************************************
 * Contains animation drawing logic and the function which calls them
 * Written by Mike Dillmann
 * TODO and Ideas:
 * 1. Add more randomization
 * 2. Global settings should be settable from external interface (e.g. RasPi) -DONE
 *
 ******************************************************************************/
#include "../include/anims.h"
#include "../include/driver.h"
#include "../include/math.h"

/*
===================
 A_Animate

 Main loop for animation control and sequencing.
===================
*/
void A_Animate() {
    // Ensure we start out with a clear frame
    D_ClrFrame();
    D_FBUpdate();

    // Loop forever
    while ( 1 ) {
        // Main draw loop; draw this animation until all frames drawn or if run
        // forever flag is set for this animation
        while ( ( ptrAnim->it < ptrAnim->its 
                || TSTFLAG( ptrAnim->FLAGS, ANIM_RUN_FOREVER ) ) ) {
            // if animation is cancelled during run, fall through and move to next animation
            if ( TSTFLAG ( ptrAnim->FLAGS, ANIM_DISABLED )
                || TSTFLAG ( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
           //     A_AnimChange();
                break;
            }
                if ( frmCtr.dn || TSTFLAG( ptrAnim->FLAGS, ANIM_NO_DELAY ) ) {
                mT4IntEnable( 0 );
                // draw one frame of current animation
                A_Launch( ptrAnim->id );
                // update main display buffer
                D_FBUpdate();
                frmCtr.dn = 0;
        //        frmCtr.acc = 0;
                ptrAnim->it++;
                mT4IntEnable( 1 );
            }
        }

        A_AnimChange(); 
    }
}

/*
===================
 A_Launch

 Animation launcher
===================
*/
void A_Launch( int anim ) {
    // delay counter for advancing color index
    int tmp;

    switch ( anim ) {
        case ( AN_SINE_2D ):
            A_SineWave2D();
            break;

        case ( AN_SINE_3D ):
            A_SineWave3D();
            break;

        case ( AN_HELIX ) :
            A_Helix();
            break;

        case ( AN_DBL_HELIX ) :
            A_DoubleHelix();
            break;

        case ( AN_DNA ) :
            A_DNA();
            break;

        case ( AN_RAIN ) :
            A_Rain();
            break;

        case ( AN_CUBE_FILL_DAZ ) :
            A_CubeFillDazzle();
            break;

        case ( AN_SPLASH ) :
            A_CubeSplash();
            break;

        case ( AN_FACE_PALETTE_SWAP ) :
            tmp = rand() % 360;
            // ensure that second color is 180 degress out of phase with the first.
            // otherwise there is a possibility of two nearly identicaly colors being picked
            // which looks sort of boring
            A_FacePaletteSwap( R_GetAngleColor( tmp ), R_GetAngleColor( tmp + 180 ) ) ;
            break;

        case ( AN_FIREWORKS) :
            A_Fireworks( 30 );
            break;

        case ( AN_TEST ):
            A_CubeTest();
            break;

        case ( AN_SA ) :
            A_SpecAnalyzer();
            break;
            
        default:
            break;
    }
}

/*
===================
 A_AnimChange

 Function to handle switching animations and housekeeping
===================
*/
void A_AnimChange( ) {
    // Disable T4 during tasks to prevent unwanted counter windups during
    mT4IntEnable( 0 );
    // First, some cleanup tasks
    // reset iteration counter
    ptrAnim->it = 0;
    // if nextAnimID was passed and is not 0, animation is being changed
    // by external device on the fly.
    if ( TSTFLAG( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
        int i = 0;
        // search the animation list until the requested ID is found
        while ( i < sizeof( anims ) ) {
            if ( anims[i].id == xsig.idNext ) {
                anIndex = i;
                break;
            }
            i++;
        }
        CLRFLAG( xsig.AFLAGS, ANIM_CHANGE_PENDING );
    } else {
        // if we reach end of the animation list, move back to first and repeat
        if ( ++anIndex > ANIM_SEQ_MAX ) {
        anIndex = 1;
        }
    }

    D_ClrFrame();
    D_FBUpdate();
    ptrAnim = &anims[anIndex];
    mT4IntEnable( 1 );   
}

/*
===================
 Name

 Desc
===================
*/
void A_Rain() {
    uint8 i, r_i, r_ii, rnd_num;
    rnd_num = rand() % 5;
    D_Shift( 'Z', SHIFT_DOWN );
    for ( i=0; i < rnd_num; i++ ) {
        r_i = rand() % 8;   // random x-coord voxels
        r_ii = rand() % 8;  // random y-coord voxels
        V_Set( r_i, r_ii, 7, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
    }   
}

/*
===================
 A_SineWave2D

 Draws a 2-D surface sine wave.
===================
*/
void A_SineWave2D() {
    static float phi;
    static uint8 x, y, Z;
    float z;

    // Todo: add amplitude adjustment
    D_ClrFrame();
    for ( x = 0; x <= CUBE_X_MAX; x++ ) {
        rgb_t clr = R_GetAngleColor( ptrAnim->rgbOpts.angle );
        z = sin( x + phi );
        Z = D_Round( D_Remap( z, -1, 1, 1, 6 ) );
        V_Set( x, 0, Z, clr );
        V_Set( x, 1, Z, clr );
        V_Set( x, 2, Z, clr );
        V_Set( x, 3, Z, clr );
        V_Set( x, 4, Z, clr );
        V_Set( x, 5, Z, clr );
        V_Set( x, 6, Z, clr );
        V_Set( x, 7, Z, clr );
    }

    phi += PI/7;
    if (phi >= 2*PI)
        phi -= 2*PI;

   
}

/*
===================
 A_SineWave3D

 Draws a 3-D surface sine wave.
===================
*/
void A_SineWave3D() {
    static uint8 x, y, Z;
    static float phi;
    float i, j, z;

    // Todo: add amplitude adjustment
    D_ClrFrame();
    for ( x = 0; x <= CUBE_X_MAX; x++ ) {
        for ( y = 0; y <= CUBE_Y_MAX; y++  ) {
            i = D_Remap( x, 0, 7, -PI, PI );
            j = D_Remap( y, 0, 7, -PI, PI );
            z = sin( ( sqrt( i*i + j*j ) ) + phi );
            Z = D_Round( D_Remap( z, -1, 1, 0, CUBE_Z_MAX ) );
            V_Set( x,y,Z,  R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
         }
    }
   
    // note: The smaller the phase increment, the 'slower' the draw speed
    // due to resolution restrictions of display, points on surface get
    // truncated which presents as a slower speed.
    phi += PI/7;
    if ( phi >= 2*PI ) {
        phi -= 2*PI;
    }
}

/*
===================
 A_Helix

 Draws a single 3D Helix around the cube perimeter
===================
*/
void A_Helix() {
    // Parameterization of Helix:
    // x(t) = a*cos(t)
    // y(t) = a*sin(t)
    // z(t) = b*t
    static uint8 x, y, z;
    static float _x, _y, _z, t;

    /*
     * Because of the aliasing, I can't really use the z(t) = t parameter
     * because the 'curve' doesn't look smooth at all. Shifting up
     * after each frame provides a faithful looking helix, but sadly I can't
     * adjust the pitch in this way. Might explore a better way to do this in
     * the future
     */
    D_Shift( 'Z', SHIFT_UP );
   _x = cos( t );
   _y = sin( t );
    x = D_Round( D_Remap( _x, -1, 1, 0, 7 ) );
    y = D_Round( D_Remap( _y, -1, 1, 0, 7 ) );
    V_Set( x, y, 0, R_GetAngleColor( 0 ) );

    if (t <= 2*PI) {
       t += PI/12;
    } else {
        t -= 2*PI;
    }
}

/*
===================
 A_Helix

 Draws a 3D Double-Helix around the cube perimeter
===================
*/
void A_DoubleHelix() {
    static uint8 x, y;
    static float _x, _y, t;
    // Shift previous frame up one first.
    D_Shift('Z', SHIFT_UP);
    _x = cos( t );
    _y = sin( t );
    x = D_Round( D_Remap( _x, -1, 1, 0, 7 ) );
    y = D_Round( D_Remap( _y, -1, 1, 0, 7 ) );
    V_Set( x, y, 0, R_GetAngleColor( 0 ) );

    // all we need to do here is add a 180 deg. phase offset to create the double helix
    _x = cos( t + PI );
    _y = sin( t + PI );
    x = D_Round( D_Remap( _x, -1, 1, 0, 7 ) );
    y = D_Round( D_Remap( _y, -1, 1, 0, 7 ) );
    V_Set( x, y, 0, R_GetAngleColor( 170 ) );

    if (t <= 2*PI) {
       t += PI/12;
    } else {
        t -= 2*PI;
    }
}

/*
===================
 A_DNA

 Draws a double-helix around the cube perimeter connected at midpoints to
 visually simulate the look of a DNA molecule.
===================
*/
void A_DNA() {
    uint8 x, y, xx, yy;
    static float _x, _y;
    static float t;

    // This draws a circle clipped to edges of cube
    // ...halfway there
    D_Shift('Z', SHIFT_UP);
    _x = cos( t );
    _y = sin( t );
    x = D_Round( D_Remap( _x, -1, 1, 1, 6 ) );
    y = D_Round( D_Remap( _y, -1, 1, 1, 6 ) );

    // all we need to do here is add a 180 deg. phase offset to create the double helix
    _x = cos( t + PI );
    _y = sin( t + PI );
    xx = D_Round( D_Remap( _x, -1, 1, 1, 6 ) );
    yy = D_Round( D_Remap( _y, -1, 1, 1, 6 ) );

    // draw the line connecting the (x,y,z) points on this frame
    // we need to draw the line first or else the line color will overwrite
    // the outside x,y point on the helix
    D_Line( x, y, 0, xx, yy, 0, R_GetAngleColor( 250 ) );
    V_Set( x, y, 0, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
    V_Set( xx, yy, 0, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );

    if ( t <= 2*PI ) {
       t += PI/12;
    } else {
        t -= 2*PI;
    }
}

/*
===================
 Name

 Desc
===================
*/
void A_Hyperboloid() {
    // Parameterization of one-sheeted hyperboloid:
    // x(t) = a*cosh(v)cos(0)
}

/*
===================
 Name

 Desc
===================
*/
void A_EllipParabloid() {

}

/*
===================
 Name

 Desc
===================
*/
void A_HyperParaboloid() {

}

/*
===================
 Name

 Desc
===================
*/
void A_CubeFillDazzle() {
    int16 i, ii, j;
    
    if ( ptrAnim->it == 0 ) {
        i = ii = j = 0;
    }

    for ( j = 0; j <= 7; j++ ) {
        for ( ii = 0; ii <= 7; ii++ ) {         // Y-offset
            for ( i = 7; i >= 0; i-- ) {        // X-offset
                // pop back to A_Animate
                if ( TSTFLAG ( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
                    return;
                }
                // params: plane, start, end, ortho-offset, perp-offset
                V_Fly( "XY", 0, i, ii, j );
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
void A_Bitmap(char bitmap) {
    int x, z;
    unsigned char chr[8];   // stores the current bitmap

    // Get the current character in the pointer and move it into 'chr'
    F_GetBitmap ( bitmap, chr );
    // Put a character on 'front' of cube.
    for ( x = 0; x < 8; x++ ) {
        for ( z = 0; z < 8; z++ ) {
            if ( chr[7-z] & ( 0x80 >> x ) ) {
                V_Set( x, 0, z, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
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
void A_PathBitmap (char bitmap) {
    uint16 z, i, ii;
    unsigned char path[28];
    F_GetPath(0, path, 28);

    for (ii = 0; ii < 8; ii++) {
        for (z = 0; z < 8; z++) {
            // Issue with drawing first few frames...
            if (F_GetBitmapPixel(bitmap,(7-z),ii)) {
                // solid RGB now for testing
                  V_Set( 0,7,z, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
                  D_FBUpdate();
            }
            else
                  V_Clr(0,7,z, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
        }                        
//         while (!(drvDelayms(g_animOpts.delay))) {}
         A_PathMove( path, 28 );
         D_FBUpdate();

    }

    for (ii = 0; ii < 20; ii++) {
//            while (!(drvDelayms(g_animOpts.delay))) {}
            A_PathMove( path, 28 );
            D_FBUpdate();
    }    

    for (ii = 0; ii < 10; ii++) {
//            while (!(drvDelayms(g_animOpts.delay))) {}
            A_PathMove( path, 28 );
            D_FBUpdate();
    }
}

/*
===================
 Name

 Desc
===================
*/
void A_PathMove( unsigned char *path, int length ) {
    int i,z;
    bool state;

    for ( i = (length-1); i >= 1; i-- ) {
 //       for (i = (length); i <= length; i++) {
        for ( z = 0; z <= 7; z++ ) {
            state = V_State(((path[(i-1)] >> 4) & 0x0F), (path[(i-1)] & 0x0F), z);
            V_Flip(((path[i] >> 4) & 0x0F), (path[i] & 0x0F), z, state);
        }
    }
    for ( i = 0; i <= 7; i++ ) {
        V_Clr( ((path[0] >> 4) & 0x0F), (path[0] & 0x0F), i );
    }
}

/*
===================
 Name

 Desc
===================
*/
void A_CrossHatch( void ) {
/*
 * 1. For each face of the cube, determine a random selection of voxels on plane of that face
 * to start with. Face order, relative to front face with origin at front-left
 *  a. front
 *  b. left
 *  c. rear
 *  d. right
 *  e. bottom
 *  f. top
 *
 * 2. On first frame, above random voxels are drawn. For each successive frame, the
 * active voxels will travel in the opposite direction of the starting face. For
 * this version, all travel at same speeds. Future: each voxels will travel at an arbitary
 * random speed
 *
 * 3. After three (or four? whichever looks better once tested...) frames have been
 * drawn, reloop over the cube faces (per 1. above) and get another batch of random voxels.
 * The voxels currently in transit will continue to move until they hit the opposite face after
 * which point they'll be cleared
 *
 */
    // Respin cube face voxels on first frame or every 4th frame
    if ( ( ptrAnim->it == 1 ) || ( ptrAnim->it % 4 ) == 0 ) {
        uint8 i, r_i, r_ii, rnd;
            // 1. front face voxels - y = 0, x & z random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( r_i, 0, r_ii, R_GetAngleColor( 0 ) );
        }

        // 2. rear face voxels - y = 7, x & z random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( r_i, 7, r_ii, R_GetAngleColor( 0 ) );
        }

        // 3. left face voxels - x = 0, y & z random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( 0, r_i, r_ii, R_GetAngleColor( 0 ) );
        }

        // 4. right face voxels - x = 7, y & z random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( 7, r_i, r_ii, R_GetAngleColor( 0 ) );
        }

        // 5. bottom face voxels - z = 0, x & y random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( r_i, r_ii, 0, R_GetAngleColor( 0 ) );
        }

        // 6. top face voxels - z = 7, x & y random
        rnd = rand() % 5;
        for (i=0; i < rnd; i++) {
            r_i = rand() % 8;   // random x-coord voxels
            r_ii = rand() % 8;  // random y-coord voxels
              V_Set( r_i, r_ii, 0, R_GetAngleColor( 0 ) );
        }
    }
}

/*
===================
 Name

 Desc
===================
*/
void A_FacePaletteSwap( rgb_t color1, rgb_t color2 ) {
    static uint8 j, x;
    // [2][64] -> [0][i] = y, i ; [1][i] = z, i
    static uint8 r_yz[64][2];
    // on first frame...
    if ( ptrAnim->it == 0 ) {
        uint8 i;
        // start at coord (0,0,0)
        x = 0;
        j = 0;
        // clear the randomized buffer
        memset( r_yz, 0, sizeof( r_yz ) );
        // Activate cube planes on x = 0 and x = 7
        // set planes to pink & blue respectively for testing
            D_ClrFrame();
            D_FillPlaneX( 0, color1 );
            D_FillPlaneX( 7, color2 );    
        // Get a somewhat-random list of numbers from 0 to 7 for y & z coords
        // - 64 total
        // The below generates a 64x2 matrix to hold 64 unique x,y coord values.
        // each x/y offset in the array - [i][0] & [i][1], respectively -
        // will be initially generated corresponding to non-randomized values
        // Each 'column' of the 'matrix' will be shuffled to obtain
        // a pseudo-random arrangement of x,y coords.
        // Kind of pulled this out of my ass so hope it's not too ugly
        for ( i = 0; i < 8; i++ ) {
            if ( TSTFLAG ( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
                return;
            }
            r_yz[8*i][0] = i;
            r_yz[8*i][1] = 0;
            r_yz[8*i+1][0] = i;
            r_yz[8*i+1][1] = 1;
            r_yz[8*i+2][0] = i;
            r_yz[8*i+2][1] = 2;
            r_yz[8*i+3][0] = i;
            r_yz[8*i+3][1] = 3;
            r_yz[8*i+4][0] = i;
            r_yz[8*i+4][1] = 4;
            r_yz[8*i+5][0] = i;
            r_yz[8*i+5][1] = 5;
            r_yz[8*i+6][0] = i;
            r_yz[8*i+6][1] = 6;
            r_yz[8*i+7][0] = i;
            r_yz[8*i+7][1] = 7;
        }

        // now shuffle the matrix columns
        // 128 passes should suffice for 'good enough' results
        // without being overly complicated
        for ( i = 0; i < 128; i++ ) {
            if ( TSTFLAG ( xsig.AFLAGS, ANIM_CHANGE_PENDING ) ) {
                return;
            }
            uint8 r1, r2, tmp_x, tmp_y;
            r1 = rand() % 64;  // random src element for swapping
            r2 = rand() % 64;  // random dest element for swapping
            // swap the two indices with the randomized indices
            // would memcpy work here??
            tmp_x = r_yz[r2][0];
            tmp_y = r_yz[r2][1];
            r_yz[r2][0] = r_yz[r1][0];
            r_yz[r2][1] = r_yz[r1][1];
            r_yz[r1][0] = tmp_x;
            r_yz[r1][1] = tmp_y;
        }
    } // first frame

    if ( x < 7 ) {
        // Note: on third iteration, only swap the two voxels
        // are next to each other - voxel 1 at (3, y) & voxel 2 at (4,y)
        // otherwise, one gets 'eaten up' during the exchange
        if ( x == 3 ) {
            V_Swap (x, r_yz[j][0], r_yz[j][1], x+1, r_yz[j][0], r_yz[j][1] );
        } else {
            // increment x-axis for voxel flying from the left face
            V_Mov( x, r_yz[j][0], r_yz[j][1], x+1, r_yz[j][0], r_yz[j][1] );
            // decrement x-axis for voxel flying from the right face
            V_Mov( 7-x, r_yz[j][0], r_yz[j][1], 7-x-1, r_yz[j][0], r_yz[j][1] );
        }
        x++;
    } else {
        // Reset x and move to next random y,z pair in array
        x = 0;
        j++;
    }     
}

/*
===================
 Name

 Desc
===================
*/
void A_VUMeter( void ) {
   
}

/*
===================
 Name

 Desc
===================
*/
void A_SpecAnalyzer( void ) {
    if ( TSTFLAG( ptrAnim->FLAGS, ANIM_FIRST_RUN ) ) {
        S_Init();
        CLRFLAG( ptrAnim->FLAGS, ANIM_FIRST_RUN );
    }

    S_ComputeFFT();
    S_DrawSpectrum();    
}

/*
===================
 Name

 Desc
===================
*/
void A_Fireworks( int n ) {

    int i,f,e;

    float origin_x = 3;
    float origin_y = 3;
    float origin_z = 3;

    int rand_y, rand_x, rand_z;

    float slowrate, gravity;

    // Particles and their position, x,y,z and their movement, dx, dy, dz
    float particles[n][6];

    origin_x = rand()%4;
    origin_y = rand()%4;
    origin_z = rand()%2;
    origin_z +=5;
    origin_x +=2;
    origin_y +=2;

    // shoot a particle up in the air
    for (e = 0; e < origin_z; e++) {
      V_Set(origin_x,origin_y,e, R_GetAngleColor( ptrAnim->rgbOpts.angle ));
      D_Delayms(600+500*e);
      D_ClrFrame();  // Clear cube
      D_FBUpdate();
    }

    // Fill particle array
    for (f = 0; f < n; f++) {
      // Position
      particles[f][0] = origin_x;
      particles[f][1] = origin_y;
      particles[f][2] = origin_z;

      rand_x = rand()%200;
      rand_y = rand()%200;
      rand_z = rand()%200;

      // Movement
      particles[f][3] = 1-(float)rand_x/100; // dx
      particles[f][4] = 1-(float)rand_y/100; // dy
      particles[f][5] = 1-(float)rand_z/100; // dz
    }

    // explode
    for (e = 0; e < 25; e++){
      slowrate = 1+tan((e+0.1)/20)*10;
      gravity = tan((e+0.1)/20)/2;

      for (f=0; f<n; f++) {
        particles[f][0] += particles[f][3]/slowrate;
        particles[f][1] += particles[f][4]/slowrate;
        particles[f][2] += particles[f][5]/slowrate;
        particles[f][2] -= gravity;

        V_Set( particles[f][0], particles[f][1], particles[f][2], R_GetAngleColor( ptrAnim->rgbOpts.angle )) ;
      }
    }
}

// Draws atoms that spin around a central point
void A_Atom( uint8 num_atoms, uint16 speed ) {

}

/*
===================
 Name

 Desc
===================
*/
void A_CubeSplash( void ) {
    F_SendText( "DILL'S", R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
    F_SendText( "R", R_SetColorRGB( RGB_RED ) );
    F_SendText( "G", R_SetColorRGB( RGB_GREEN ) );
    F_SendText( "B", R_SetColorRGB( RGB_BLUE ) );
    F_SendText( "CUBE", R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
}

/*
===================
 Name

 Desc
===================
*/
void A_CubeTest( void ) {
    int i, j;

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 0, R_SetColorRGB( RGB_RED ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 1, R_SetColorRGB( RGB_GREEN ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 2, R_SetColorRGB( RGB_BLUE ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
           V_Set( i, j, 3, R_SetColorRGB( RGB_ORANGE ) );
        }
    }

     for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
           V_Set( i, j, 4, R_SetColorRGB( RGB_VIOLET ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 5, R_SetColorRGB( RGB_YELLOW ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 6, R_SetColorRGB( RGB_CYAN ) );
        }
    }

    for ( i = 0; i <= 7; i++ ) {
        for ( j = 0; j <= 7; j++ ) {
            V_Set( i, j, 7, R_SetColorRGB( RGB_MAGENTA ) );
        }
    }
}

/**********************************************************************/
/*
 * Everything below this comment is for TESTING/EXPERIMENTATION ONLY and
 * will be removed in the final build!!
 *
 */
/**********************************************************************/
void A_CircleRotate(uint8 radius, uint8 c_x, uint8 c_y, uint8 c_z) {
    static rgb_t tmpBuff[8][8][8];
    VECTOR4 pointsColVec, newVec;
    MATRIX4 rotMx;
    int32 x, y, z, _xx, _yy, _zz;

    // For offseting the cube center point around which the rotation
    // is to occur
    // Rotation angles
    static float phi0, phi1, phi2;

    // Clear the buffers first
    memset(tmpBuff, 0, sizeof(tmpBuff));
    memset(g_colorBuffer, 0, sizeof(g_colorBuffer));
    /* Though extremely inefficient, I need to RGB_REDraw the circle and rotate
     from scratch on each call to the function because over successive
     transformations, the image will become corrupted due to roundoff error.
     RGB_REDrawing it each frame will minimize the roundoff effects over
     multiple iterations, and doesn't seem to slow the animation down at all.
     */
    D_Circle(4, c_x, c_y, c_z);
    // Get the rotation matrix
    rotMx = M_Rotate3D(phi0, phi1, phi2);

    for (z = 0; z <= CUBE_Z_MAX; z++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (x = 0; x <= CUBE_X_MAX; x++) {
                // Is this voxel set? If not, don't apply transform
                if (V_State(x, y, z)) {
                      // we need to offset the points prior to rotation transform to ensure we rotate around the mid-point
                      // not around some offset from the origin
                    pointsColVec.vec[0] = (x-c_x);
                    pointsColVec.vec[1] = (y-c_y);
                    pointsColVec.vec[2] = (z-c_z);
                    // 4th component is always 1
                    pointsColVec.vec[3] = 1;
                    // Apply rotation and get new voxel position
                    newVec = M_MulVec3D(rotMx, pointsColVec);
                    // offset the points back to their oriignal location on the cube
                    _xx = newVec.vec[0] + c_x;
                    _yy = newVec.vec[1] + c_y;
                    _zz = newVec.vec[2] + c_z;
                     // check new voxel position for bounds
                    if (D_Bounds(_xx, _yy, _zz))
                        // Update temp buffer with voxel data
                        tmpBuff[_xx][_yy][_zz] = g_colorBuffer[x][y][z];

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

    // Move the modified buffer back to the color buffer
    memcpy(g_colorBuffer, tmpBuff, sizeof(tmpBuff));

    // Update the rotation angle
    phi0 += PI/2;
    if (phi0 >= 2*PI)
        phi0 -= 2*PI;
}

void A_Translate2D(void) {
 /* Testing matrix functions
  * 1. Test translate function in 2D
  * 2. Test scale function in 2D
  * 3. Test rotate function in 2D
  * 4. Test functions above in 3D
  * Set a voxel or a few voxels and apply the appropriate transormations as needed. 
  * NOTE: recall order of operations for column-major: scale->rotate->translate (not commutative!)
  */
    // we're interested in doing 2D transforms only, so we need a 3D column vector
    // [0] = x
    // [1] = y
    // [2] = w = 1
    rgb_t tmpBuff[8][8][8];
    VECTOR3 pointsColVec, newVec;
    MATRIX3 tmpMx;
    int32 x, y, z;
   // First test - set (0,0,0) and translate to (3,0,0). This should proof out at least bare-bones functionality
   // 1a. set the voxel
    
    V_Set(0,0,0,RGB_RED);
    V_Set(1,0,0,RGB_RED);
    V_Set(2,0,0,RGB_RED);
   // 1b. Apply the translation - shift 3 units up the x-axis
    tmpMx = M_Translate2D(3, 3);
 
   // Now that we have translation matrix, multiply each voxel by matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
   // Note we want to move in descending order through the voxel loop. If we move in ascending, any already-translated voxels
   // might be translated again due to false-positive triggering by VXState.
   for (z = CUBE_Z_MAX; z >= 0; z--) {
        for (y = CUBE_Y_MAX; y >= 0; y--) {
            for (x = CUBE_X_MAX; x >= 0; x--) {
                // Is this voxel set? If not, don't apply transform
                if (V_State(x, y, z)) {
                    // Set up the column vector for this point
                    pointsColVec.vec[0] = x;
                    pointsColVec.vec[1] = y;
                    pointsColVec.vec[2] = 1;
                   //  pointsColVector.vec[0] = 3;
                   //  pointsColVector.vec[1] = 0;
                    // Get new voxel position.
                    newVec = M_MulVec2D(tmpMx, pointsColVec);
                    // check new voxel position for bounds
                    if (D_Bounds(newVec.vec[0], newVec.vec[1], 0))
                        // Swap the vox positions
                        V_Swap(x, y, z, newVec.vec[0], newVec.vec[1], z);

                    else
                    /* For now just clear the voxel, but note this destroys any animation data
                     * outside of the current boundary which cannot be retrieved if
                     * the animation continues to move. May want to use a
                     * larger array and clip to the cube boundaries to allow for
                     * more headroom, but this will chew up a lot of cycles...
                     * To keep things simple for now, just ensure all post-transform
                     * coordinates will fit inside the cube space
                     */
                        V_Clr(x, y, z);
               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis
}

// Working [finally...]
/* NOTE - THE BELOW IS FOR TESTING NEW FUNCTIONALITY AND WILL BE REMOVED */
void anRotate2D(void) {
 /* Testing matrix functions
  * 1. Test translate function in 2D
  * 2. Test scale function in 2D
  * 3. Test rotate function in 2D
  * 4. Test functions above in 3D
  * Set a voxel or a few voxels and apply the appropriate transormations as needed.
  * NOTE: recall order of operations for column-major: scale->rotate->translate (not commutative!)
  */
    // we're interested in doing 2D transforms only, so we need a 3D column vector
    // [0] = x
    // [1] = y
    // [2] = w = 1
    static rgb_t tmpBuff[8][8][8];
    VECTOR3 pointsColVec, newVec;
    MATRIX3 tmpMx;
    int32 x, y, z;
    int32 _xx, _yy, _zz;
    static float phi;
   // First test - set (0,0,0) and translate to (3,0,0). This should proof out at least bare-bones functionality
   // 1a. set the voxel

    // NOTE: ok so this is a quick hack to purge the buffers before drawing new voxels.
    // Without the purge, corruption happens
    memset(tmpBuff, 0, sizeof(tmpBuff));
    memset(g_colorBuffer, 0, sizeof(g_colorBuffer));

 // Test pattern - simple rectangle
    V_Set(1,2,0,RGB_RED);
    V_Set(2,2,0,RGB_RED);     // this is the middle point, rotate around this one
    V_Set(3,2,0,RGB_RED);
    V_Set(1,3,0,RGB_RED);
    V_Set(2,3,0,RGB_RED);
    V_Set(3,3,0,RGB_RED);
 
   // 1b. Apply the translation - shift 3 units up the x-axis
   //   tmpMx = MXRotate2D(0);
      tmpMx = M_Rotate2D(phi);

   
   // Now that we have rotation matrix, multiply each voxel by the matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
    for (z = 0; z <= CUBE_Z_MAX; z++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (x = 0; x <= CUBE_X_MAX; x++) {
                // Is this voxel set? If not, don't apply transform
                if (V_State(x, y, z)) {

                      pointsColVec.vec[0] = (x-2);
                      pointsColVec.vec[1] = (y-2);
                      pointsColVec.vec[2] = 1;
                    // Apply rotation and get new voxel position
                    newVec = M_MulVec2D(tmpMx, pointsColVec);
                    _xx = newVec.vec[0] + 2;
                    _yy = newVec.vec[1] + 2;
                     // check new voxel position for bounds
                    if (D_Bounds(_xx, _yy, 0))
                        // Update temp buffer with voxel data
                        tmpBuff[_xx][_yy][z] = g_colorBuffer[x][y][z];

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

    memcpy(g_colorBuffer, tmpBuff, sizeof(tmpBuff));

    phi += 2*PI/6;
    if (phi >= 2*PI)
        phi -= 2*PI;
   // Delay before next frame
//   while (!(drvDelayms(g_animOpts.delay))) {}
}

// Working, but works best with larger angle divisions (PI/2, PI/4), etc depending
// On the complexity and geometry of the shape being rotated.
/* NOTE - THE BELOW IS FOR TESTING NEW FUNCTIONALITY AND WILL BE REMOVED */
void anRotate3D(void) {
 /* Testing matrix functions
  * 1. Test translate function in 2D
  * 2. Test scale function in 2D
  * 3. Test rotate function in 2D
  * 4. Test functions above in 3D
  * Set a voxel or a few voxels and apply the appropriate transormations as needed.
  * NOTE: recall order of operations for column-major: scale->rotate->translate (not commutative!)
  */
    // we're interested in doing 2D transforms only, so we need a 3D column vector
    // [0] = x
    // [1] = y
    // [2] = w = 1
    static rgb_t tmpBuff[8][8][8];
    VECTOR4 pointsColVec, newVec;
    MATRIX4 tmpMx;
    int32 x, y, z;
    int32 _xx, _yy, _zz;
    static float phi, alpha, beta;

    // NOTE: ok so this is a quick hack to purge the buffers before drawing new voxels.
    // Without the purge, corruption happens. Need to find a better way...
    memset(tmpBuff, 0, sizeof(tmpBuff));
    memset(g_colorBuffer, 0, sizeof(g_colorBuffer));
    // Quick'n'dirty mini cube pattern. Yes, I know this is ugly but it's just for testing...
    D_Line(2,2,2,2,4,2,R_GetAngleColor( 0 ));
    D_Line(4,2,2,4,4,2,R_GetAngleColor( 0 ));
    D_Line(2,2,4,2,4,4,R_GetAngleColor( 0 ));
    D_Line(4,2,4,4,4,4,R_GetAngleColor( 0 ));
    V_Set(2,2,3,RGB_RED);
    V_Set(2,4,3,RGB_RED);
    V_Set(4,2,3,RGB_RED);
    V_Set(4,4,3,RGB_RED);
    V_Set(3,2,2,RGB_RED);
    V_Set(3,2,4,RGB_RED);
    V_Set(3,4,2,RGB_RED);
    V_Set(3,4,4,RGB_RED);

    tmpMx = M_Rotate3D(phi,alpha,beta);
 //     tmpMx = MXRotate3D(0,phi,0);

   // Now that we have rotation matrix, multiply each voxel by the matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
    for (z = 0; z <= CUBE_Z_MAX; z++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (x = 0; x <= CUBE_X_MAX; x++) {
                // Is this voxel set? If not, don't apply transform
                if (V_State(x, y, z)) {
                      // we need to offset the points prior to rotation transform to ensure we rotate around the mid-point
                      // not around some offset from the origin
                    pointsColVec.vec[0] = (x-2);
                    pointsColVec.vec[1] = (y-2);
                    pointsColVec.vec[2] = (z-3);
                    pointsColVec.vec[3] = 1;
                    // Apply rotation and get new voxel position
                    newVec = M_MulVec3D(tmpMx, pointsColVec);
                    // offset the points back to their oriignal location on the cube
                    _xx = newVec.vec[0] + 3;
                    _yy = newVec.vec[1] + 3;
                    _zz = newVec.vec[2] + 3;
                     // check new voxel position for bounds
                    if (D_Bounds(_xx, _yy, _zz))
                        // Update temp buffer with voxel data
                        tmpBuff[_xx][_yy][_zz] = g_colorBuffer[x][y][z];

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

    memcpy(g_colorBuffer, tmpBuff, sizeof(tmpBuff));

    phi += PI/2;
    if (phi >= 2*PI)
        phi -= 2*PI;

    alpha += PI/4;
    if (alpha >= 2*PI)
        alpha -= 2*PI;

    beta += PI/6;
    if (beta >= 2*PI)
        beta -= 2*PI;
}
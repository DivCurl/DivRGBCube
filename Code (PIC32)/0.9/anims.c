#include "anims.h"
#include "driver.h"

// Animation Sequence Handler
void Animate(uint8 index, uint16 its) {
    int32 count = 0;       // current iteration count
    ANIMATION anim = index;

    switch (anim) {
        case (SINE_2D):
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                RGBRefreshColorBuffer();
                ANSineWave2D();
                count++;
            }
            break;
        case (SINE_3D) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                RGBRefreshColorBuffer();
                ANSineWave3D();
                count++;
            }
            break;
        case (RAIN) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                RGBRefreshColorBuffer();
                ANRain();
                count++;
            }
            break;
        case (CUBE_FILL_DAZ) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                RGBRefreshColorBuffer();
                ANCubeFillDazzle();
                count++;
            }
            break;
        case (TEXT_FLY) :
            while (count < its) {
                g_rgb_opts.delay = 180;
                g_rgb_opts.fade_mode = SOLID_COLOR;                
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("M");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("E");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("R");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("R");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("Y");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("C");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("H");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("R");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("I");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("S");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("T");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("M");
                g_rgb_opts.opt1 = RED;
                RGBRefreshColorBuffer();
                ANSendText("A");
                g_rgb_opts.opt1 = GREEN;
                RGBRefreshColorBuffer();
                ANSendText("S");
                count++;
            }
            break;
        // Just a stub for testing new functionality so I don't have to dick with main()'s scope
        case (TEST):
            // For testing matrix translation functions
 //         AnimTranslate2D();
           ANRotate3D();
            while (!(DRVDelayms(600))) {}
            break;
        case (CUBE_TEST):
            ANCubeTest();
            break;
        default:
            break;
    }
}

// Working
void ANRain() {
    uint8 i, r_i, r_ii, rnd_num;
    
    g_rgb_opts.its = 250;
    g_rgb_opts.fade_mode = FADE_CW_CONT;
    g_rgb_opts.fade_delay = 10;
    g_rgb_opts.angle_roc = 2;
    
    while (!(DRVDelayms(100))) {}
    rnd_num = rand()%5;
    for (i=0; i < rnd_num; i++) {
        r_i = rand()%8;
        r_ii = rand()%8;
          VXSet(r_i, r_ii, 7, RGBGetColor(r_i, r_ii, 7));
    }
    // Note: framebuffer update is handled in Shift() function
    DRVShift('Z', DOWN);
}

// Working
void ANSineWave2D() {
    static float phi;
    static uint8 x, y, Z;
    float z;
    g_rgb_opts.fade_delay = 500;
    g_rgb_opts.angle_roc = 2;

    while (!(DRVDelayms(150))) {}
    DRVClrFrame();
    for (x = 0; x <= X_MAX; x++ ) {

        z = sin(x + phi); 
        Z = DRVRound(DRVRemap(z, -1, 1, 0, Z_MAX));
        VXSetSolid(x, 1, Z, BLUE);
        VXSet(x, 2, Z, RGBGetColor(x, x, Z));
        VXSet(x, 3, Z, RGBGetColor(x, x, Z));
        VXSet(x, 4, Z, RGBGetColor(x, x, Z));
        VXSet(x, 5, Z, RGBGetColor(x, x, Z));
        VXSetSolid(x, 6, Z, BLUE);
    }

    phi += PI/8;
    if (phi >= 2*PI)
        phi -= 2*PI;

    DRVFBUpdate();
}

// Working
void ANSineWave3D() {
    static uint8 x, y, Z;
    static float phi;
    float i, j, z;

    g_rgb_opts.fade_delay = 10;
    g_rgb_opts.angle_roc = 2;

    while (!(DRVDelayms(100))) {}
    DRVClrFrame();   // Clear the current frame before updating it
    for (x = 0; x <= X_MAX; x++ ) {
        for (y = 0; y <= Y_MAX; y++) {
            i = DRVRemap(x, 0, 7, -PI, PI);
            j = DRVRemap(y, 0, 7, -PI, PI);
            z = sin((sqrt(i*i + j*j)) + phi );
            Z = DRVRound(DRVRemap(z,-1,1,0,Z_MAX));
            VXSet(x,y,Z, RGBGetColor(x, y, Z));
         }
    }
    DRVFBUpdate();
    
    // note: The smaller the phase increment, the slower the draw speed
    phi += PI/7;
    if (phi >= 2*PI)
        phi -= 2*PI;  
 }

/* Fill cube one voxel at time
 * Start by sending a voxel flying down x=0, z=0 axis and stop at (0,7,0).
 * Continue filling this axis until all y-voxels on x=0 are filled. Then move to x=1, z=0 axis.
 * Continue filling until all voxels on this plane are filled, then move up to next plane and
 * repeat until entire cube is filled
 */
// Working
void ANCubeFillDazzle() {
    int16 i, ii, j;

    g_rgb_opts.fade_delay = 250;
    g_rgb_opts.angle_roc = 2;

    DRVClrFrame();

    for (j = 0; j <= 7; j++) {
        for (ii = 0; ii <= 7; ii++) {         // Y-offset
            for (i = 7; i >= 0; i--) {        // X-offset
                // params: plane, start, end, ortho-offset, perp-offset, delay
                VXFly("XY", 0, i, ii, j, 25);
                RGBRefreshColorBuffer();
            }
        }
    }
}

// Working, WIP. Need more flexible way to set color for this function
void ANSendText(const char * str) {
    int x, y, i;
    unsigned char chr[5];   // stores the current character
    
    //	While we haven't hit the null byte...
    while (*str) {
        // Get the current character in the pointer and move it into 'chr'
        FNTGetchar(*str++, chr);
    
        // Put a character on the cube.
        for (x = 0; x < 5; x++) {
            for (y = 0; y < 8; y++)	{
                if (chr[x] & (0x80 >> y)) {
                    VXSet(x+2, 0, y, RGBGetColor(x+2, 0, y));
                }
            }
        }

        for (i = 0; i < 5; i++) {
            while (!(DRVDelayms(g_rgb_opts.delay))) {}
            DRVShift('Y', OUT);
        }

    }   // end while


    for (i = 0; i < 8; i++) {
        while (!(DRVDelayms(g_rgb_opts.delay))) {}
        DRVShift('Y', OUT);
    }
}

// Not working, WIP
void ANPathBitmap (char bitmap) {
    int z, i, ii;
    z = 4;
    unsigned char path[28];
    FNTGetPath(0, path, 28);

    for (i=0; i < g_rgb_opts.its; i++) {
        for (ii = 0; ii < 8; ii++) {
            for (z = 0; z < 8; z++) {
                if (FNTGetBitmapPixel(bitmap,(7-z),ii))
                    // solid RGB now for testing
                        VXSetSolid(0,7,z, RED);
                else
                    VXClr(0,7,z);
            }
            while (!(DRVDelayms(g_rgb_opts.delay))) {}
            ANPathMove(path, 28);
        }

        for (ii = 0; ii < 20; ii++) {
                while (!(DRVDelayms(g_rgb_opts.delay))) {}
                ANPathMove(path, 28);
        }
    }

    for (ii = 0; ii < 10; ii++) {
            while (!(DRVDelayms(g_rgb_opts.delay))) {}
            ANPathMove(path, 28);
    }
}

// Not working, WIP
void ANPathMove(unsigned char *path, int length) {
	int i,z;
	unsigned char state;

	for (i = (length-1); i >= 1; i--) {
//	for (i = (length); i <= length; i++) {
		for (z = 0; z < 8; z++) {

	//		state = Get_Voxel(((path[(i-1)]>>4) & 0x0f), (path[(i-1)] & 0x0f), z);
	//		Alter_Voxel(((path[i]>>4) & 0x0f), (path[i] & 0x0f), z, state);
		}
	}
	for (i = 0; i < 8; i++)
            VXClr(((path[0]>>4) & 0x0f), (path[0] & 0x0f), i);
}
// Testing - WORKS!!
/* NOTE - THE BELOW IS FOR TESTING NEW FUNCTIONALITY AND WILL BE REMOVED */
void ANTranslate2D(void) {
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
    RGB tmpBuff[8][8][8];
    VECTOR3 pointsColVec, newVec;
    MATRIX3 tmpMx;
    int32 x, y, z;
   // First test - set (0,0,0) and translate to (3,0,0). This should proof out at least bare-bones functionality
   // 1a. set the voxel
    
    VXSetSolid(0,0,0,RED);
    VXSetSolid(1,0,0,RED);
    VXSetSolid(2,0,0,RED);
   // 1b. Apply the translation - shift 3 units up the x-axis
    tmpMx = MXTranslate2D(3, 3);
 
   // Now that we have translation matrix, multiply each voxel by matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
   // Note we want to move in descending order through the voxel loop. If we move in ascending, any already-translated voxels
   // might be translated again due to false-positive triggering by VXState.
   for (z = Z_MAX; z >= 0; z--) {
        for (y = Y_MAX; y >= 0; y--) {
            for (x = X_MAX; x >= 0; x--) {
                // Is this voxel set? If not, don't apply transform
                if (VXState(x, y, z)) {
                    // Set up the column vector for this point
                    pointsColVec.vec[0] = x;
                    pointsColVec.vec[1] = y;
                    pointsColVec.vec[2] = 1;
                   //  pointsColVector.vec[0] = 3;
                   //  pointsColVector.vec[1] = 0;
                    // Get new voxel position.
                    newVec = MXMulVec2D(tmpMx, pointsColVec);
                    // check new voxel position for bounds
                    if (DRVBounds(newVec.vec[0], newVec.vec[1], 0))
                        // Swap the vox positions
                        VXSwap(x, y, z, newVec.vec[0], newVec.vec[1], z);

                    else
                    /* For now just clear the voxel, but note this destroys any animation data
                     * outside of the current boundary which cannot be retrieved if
                     * the animation continues to move. May want to use a
                     * larger array and clip to the cube boundaries to allow for
                     * more headroom, but this will chew up a lot of cycles...
                     * To keep things simple for now, just ensure all post-transform
                     * coordinates will fit inside the cube space
                     */
                        VXClr(x, y, z);

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

   // Finally, update the cube buffer
   DRVFBUpdate();
}

// Working [finally...]
/* NOTE - THE BELOW IS FOR TESTING NEW FUNCTIONALITY AND WILL BE REMOVED */
void ANRotate2D(void) {
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
    static RGB tmpBuff[8][8][8];
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
    memset(g_rgb_buff, 0, sizeof(g_rgb_buff));

 // Test pattern - simple rectangle
    VXSetSolid(1,2,0,RED);
    VXSetSolid(2,2,0,RED);     // this is the middle point, rotate around this one
    VXSetSolid(3,2,0,RED);
    VXSetSolid(1,3,0,RED);
    VXSetSolid(2,3,0,RED);
    VXSetSolid(3,3,0,RED);
 
   // 1b. Apply the translation - shift 3 units up the x-axis
   //   tmpMx = MXRotate2D(0);
      tmpMx = MXRotate2D(phi);

   
   // Now that we have rotation matrix, multiply each voxel by the matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
                // Is this voxel set? If not, don't apply transform
                if (VXState(x, y, z)) {

                      pointsColVec.vec[0] = (x-2);
                      pointsColVec.vec[1] = (y-2);
                      pointsColVec.vec[2] = 1;
                    // Apply rotation and get new voxel position
                    newVec = MXMulVec2D(tmpMx, pointsColVec);
                    _xx = newVec.vec[0] + 2;
                    _yy = newVec.vec[1] + 2;
                     // check new voxel position for bounds
                    if (DRVBounds(_xx, _yy, 0))
                        // Update temp buffer with voxel data
                        tmpBuff[_xx][_yy][z] = g_rgb_buff[x][y][z];

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

    memcpy(g_rgb_buff, tmpBuff, sizeof(tmpBuff));

    phi += 2*PI/6;
    if (phi >= 2*PI)
        phi -= 2*PI;

   // Finally, update the cube buffer
   DRVFBUpdate();
}

// Working, but works best with larger angle divisions (PI/2, PI/4), etc depending
// On the complexity and geometry of the shape being rotated.
/* NOTE - THE BELOW IS FOR TESTING NEW FUNCTIONALITY AND WILL BE REMOVED */
void ANRotate3D(void) {
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
    static RGB tmpBuff[8][8][8];
    VECTOR4 pointsColVec, newVec;
    MATRIX4 tmpMx;
    int32 x, y, z;
    int32 _xx, _yy, _zz;
    static float phi;

    // NOTE: ok so this is a quick hack to purge the buffers before drawing new voxels.
    // Without the purge, corruption happens. Need to find a better way...
    memset(tmpBuff, 0, sizeof(tmpBuff));
    memset(g_rgb_buff, 0, sizeof(g_rgb_buff));
    // Quick'n'dirty mini cube pattern. Yes, I know this is ugly but it's just for testing...
    DRVLine(2,2,2,2,4,2);
    DRVLine(4,2,2,4,4,2);
    DRVLine(2,2,4,2,4,4);
    DRVLine(4,2,4,4,4,4);
    VXSetSolid(2,2,3,RED);
    VXSetSolid(2,4,3,RED);
    VXSetSolid(4,2,3,RED);
    VXSetSolid(4,4,3,RED);
    VXSetSolid(3,2,2,RED);
    VXSetSolid(3,2,4,RED);
    VXSetSolid(3,4,2,RED);
    VXSetSolid(3,4,4,RED);

      tmpMx = MXRotate3D(phi,phi,0);
 //     tmpMx = MXRotate3D(0,phi,0);

   // Now that we have rotation matrix, multiply each voxel by the matrix to get new voxel position [in column vector]
   // Swap the current voxel location with the new voxel location as long as it's within range of the cube
   // If the new voxel point is outside the cube boundary, just clear the existing voxel point
    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
                // Is this voxel set? If not, don't apply transform
                if (VXState(x, y, z)) {
                      // we need to offset the points prior to rotation transform to ensure we rotate around the mid-point
                      // not around some offset from the origin
                    pointsColVec.vec[0] = (x-2);
                    pointsColVec.vec[1] = (y-2);
                    pointsColVec.vec[2] = (z-3);
                    pointsColVec.vec[3] = 1;
                    // Apply rotation and get new voxel position
                    newVec = MXMulVec3D(tmpMx, pointsColVec);
                    // offset the points back to their oriignal location on the cube
                    _xx = newVec.vec[0] + 3;
                    _yy = newVec.vec[1] + 3;
                    _zz = newVec.vec[2] + 3;
                     // check new voxel position for bounds
                    if (DRVBounds(_xx, _yy, _zz))
                        // Update temp buffer with voxel data
                        tmpBuff[_xx][_yy][_zz] = g_rgb_buff[x][y][z];

               }  // if VXState
           } // X axis
        } // Y axis
    } // Z axis

    memcpy(g_rgb_buff, tmpBuff, sizeof(tmpBuff));

    phi += PI/2;
    if (phi >= 2*PI)
        phi -= 2*PI;

   // Finally, update the cube buffer
   DRVFBUpdate();
}

/*
 * Simple routine for testing only. Lights up all LEDs with a different color on each x-y plane.
 * Useful for ensuring BCM and basic software/hardware functions are working properly
 */
void ANCubeTest(void) {
    uint8 i, j;

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 0, RED);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 1, GREEN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 2, BLUE);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 3, ORANGE);
        }
    }

     for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 4, VIOLET);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 5, YELLOW);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 6, CYAN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VXSetSolid(i, j, 7, MAGENTA);
        }
    }
}
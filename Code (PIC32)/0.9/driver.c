#include "driver.h"

// Non-blocking millisecond delay function. Relies on T4 interrupt for clock tick counts.
uint8 DRVDelayms(uint32 delay) {

    static uint16 count, tmp = 0;

    if (g_ms_ctr != tmp) {
        tmp = g_ms_ctr;
        count++;
    }

    if (count == delay) {
        count = 0;
        tmp = 0;
        return 1;
    }
    else
        return 0;
}

// Returns absolute value of input value
// Note sure I like how this is implemented. Doesn't cast to other types...
float DRVAbs(float in) {
    return (in > 0 ? in : -in);
}

int16 DRVRound(float in) {
	int8 s = in/DRVAbs(in);
	return (int16)(s * (DRVAbs(in) + 0.5));
}

// Bounds checking of input voxel coordinate
bool DRVBounds(int32 x, int32 y, int32 z) {
    if (x >= X_MIN && x <= X_MAX &&
        y >= Y_MIN && y <= Y_MAX &&
        z >= Z_MIN && z <= Z_MAX)
        return TRUE;    
    else
        return FALSE;
}

float DRVRemap(float in, float inMin, float inMax, float outMin, float outMax) {
	float out;
	out = (in - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
	return out;
}

// Modified Brensenham's algorithm for drawing lines
void DRVLine(uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2) {
    int i, dx, dy, dz, l, m, n;
    int x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    int point[3];

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
            VXSetSolid(point[0], point[1], point[2], RED);
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
            VXSetSolid(point[0], point[1], point[2], RED);
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
            VXSetSolid(point[0], point[1], point[2], RED);
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
    
    VXSetSolid(point[0], point[1], point[2], RED);

}

void DRVFillPlaneX(uint8 X) {
    uint8 y, z;
    if (X >= 0 && X < X_MAX) {
        for (y = 0; y < Y_MAX; z++) {
            for (z = 0; z < Z_MAX; y++) {
                VXSet(X, y, z, RGBGetColor(X, y, z));
            }
        }
    }
}

void DRVClrPlaneX(uint8 X) {
    uint8 y, z;
        for (y = 0; y <= Y_MAX; y++) {
            for (z = 0; z <= Z_MAX; z++) {
                VXClr(X, y, z);
            }
        }
 }

void DRVFillPlaneY(uint8 Y) {
    uint8 x, z;
    if (Y >= 0 && Y < Y_MAX) {
        for (x = 0; x < X_MAX; x++) {
            for (z = 0; z < Z_MAX; z++) {
                VXSet(x, Y, z, RGBGetColor(x, Y, z));
            }
        }
    }
}

void DRVClrPlaneY(uint8 Y) {
    uint8 x, z;

    for (x = 0; x <= X_MAX; x++) {
        for (z = 0; z <= Z_MAX; z++) {
            VXClr(x, Y, z);
        }
    }
 }

void DRVFillPlaneZ(uint8 Z) {
    uint8 x, y;

    if (Z >= 0 && Z < Z_MAX) {
        for (x = 0; x < X_MAX; x++) {
            for (y = 0; y < Y_MAX; y++) {
                VXSet(x, y, Z, RGBGetColor(x, y, Z));
            }
        }
    }
}

void DRVClrPlaneZ(uint8 Z) {
    uint8 x, y;

    for (x = 0; x <= X_MAX; x++) {
        for (y = 0; y <= Y_MAX; y++) {
            VXClr(x, y, Z);
        }
    }
}

// Shift the entire cube framebuffer up, down, left, or right one axis increment
// Working, could stand to be optimized. Looks kinda dirty.
void DRVShift(char axis, SHIFT_DIR dir) {
    uint8 x, y, z;
    RGB rgb[8][8][8];
    // load temp buffer with current frame of cube output buffer
    memcpy(rgb, g_cube_fb, sizeof(g_cube_fb));

    switch (axis) {
       case ('X') :
            if (dir == RIGHT ) {
                for (x = X_MIN; x < X_MAX; x++) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x+1][y][z] = rgb[x][y][z];  

                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneX(X_MIN);
            }

            if (dir == LEFT ) {
                for (x = X_MAX; x > X_MIN; x--) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x-1][y][z] = rgb[x][y][z];  

                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneX(X_MAX);
            }
            break;      // Case 'X'

        case ('Y') :
            if (dir == OUT ) {
                for (y = Y_MIN; y < Y_MAX; y++) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x][y+1][z] = rgb[x][y][z];   
                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneY(Y_MIN);
            }

            if (dir == IN ) {
                for (y = Y_MAX; y > Y_MIN; y--) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x][y-1][z] = rgb[x][y][z];

                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneY(Y_MAX);
            }
            break;      // Case 'Y'

       case ('Z') :
            if (dir == UP ) {
                for (z = Z_MIN; z < Z_MAX; z++) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (x = 0; x <= X_MAX; x++) {
                            g_rgb_buff[x][y][z+1] = rgb[x][y][z]; 

                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneZ(Z_MIN);
            }

            if (dir == DOWN ) {
                for (z = Z_MAX; z > 0; z--) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (x = 0; x <= X_MAX; x++) {
                            g_rgb_buff[x][y][z-1] = rgb[x][y][z];   

                        }
                    }
                }
                DRVFBUpdate();
                DRVClrPlaneZ(Z_MAX);
            }
            break;      // Case 'Z'
        default:
            break;
    } // switch
}

 // Clears the RGB frame buffer
void DRVClrFrame() {
    memset(g_rgb_buff, 0, sizeof(g_rgb_buff));
 }

// Updates the cube output framebuffer  with data from the current framebuffer
// This prevents needing to clear out the entire framebuffer in between frame
// draws which can cause flicker at lower frame rates
void DRVFBUpdate() {
    uint8 x, y, z;
     // *** This blocks for 512 friggin instructions to complete the loop. Surely there's a better way???
    for (z = Z_MIN; z <= Z_MAX; z++) {
        for (y = Y_MIN; y <= Y_MAX; y++) {
            for (x = X_MIN; x <= X_MAX; x++) {
                if (g_rgb_buff[x][y][z].r == 0)
                    g_cube_fb[x][y][z].r = 0;
                else
                    g_cube_fb[x][y][z].r = g_rgb_buff[x][y][z].r;

                if (g_rgb_buff[x][y][z].g == 0)
                    g_cube_fb[x][y][z].g = 0;
                else
                    g_cube_fb[x][y][z].g = g_rgb_buff[x][y][z].g;

                if (g_rgb_buff[x][y][z].b == 0)
                    g_cube_fb[x][y][z].b = 0;
                else
                    g_cube_fb[x][y][z].b = g_rgb_buff[x][y][z].b;
            }
        }
    }
}    
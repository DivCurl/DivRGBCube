/* Low-level drawing and helper functions to assist in high level tasks and animation functions */
#include "driver.h"


// Non-blocking millisecond delay function. Relies on T4 interrupt for clock tick counts.
uint8 Delay_mS(uint32 delay) {

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
float Abs(float in) {
    return (in > 0 ? in : -in);
}

int16 Round(float in) {
	int8 s = in/Abs(in);
	return (int16)(s * (Abs(in) + 0.5));
}

float Remap(float in, float inMin, float inMax, float outMin, float outMax) {
	float out;
	out = (in - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;
	return out;
}

// Sets a single voxel to a solid R, G, or B color
void Voxel_Set_Solid_RGB(uint8 x, uint8 y, uint8 z, SOLID_RGB color) {

    switch (color) {
        case (RED) :
            g_rgb_buff[x][y][z].r = 255;
            g_rgb_buff[x][y][z].g = 0;
            g_rgb_buff[x][y][z].b = 0;
            break;
        case (ORANGE) :
            g_rgb_buff[x][y][z].r = 255;
            g_rgb_buff[x][y][z].g = 125;
            g_rgb_buff[x][y][z].b = 0;
            break;
        case (YELLOW) :
            g_rgb_buff[x][y][z].r = 255;
            g_rgb_buff[x][y][z].g = 255;
            g_rgb_buff[x][y][z].b = 0;
            break;
        case (GREEN) :
            g_rgb_buff[x][y][z].r = 0;
            g_rgb_buff[x][y][z].g = 255;
            g_rgb_buff[x][y][z].b = 0;
            break;
        case (CYAN) :
            g_rgb_buff[x][y][z].r = 0;
            g_rgb_buff[x][y][z].g = 255;
            g_rgb_buff[x][y][z].b = 255;
            break;
        case (BLUE) :
            g_rgb_buff[x][y][z].r = 0;
            g_rgb_buff[x][y][z].g = 0;
            g_rgb_buff[x][y][z].b = 255;
            break;
        case (VIOLET) :
            g_rgb_buff[x][y][z].r = 125;
            g_rgb_buff[x][y][z].g = 0;
            g_rgb_buff[x][y][z].b = 255;
            break; 
        case (MAGENTA) :
            g_rgb_buff[x][y][z].r = 255;
            g_rgb_buff[x][y][z].g = 0;
            g_rgb_buff[x][y][z].b = 255;
            break;
        default:
            break;
    }
}

void Voxel_Set_RGB(uint8 x, uint8 y, uint8 z, RGB color) {
    g_rgb_buff[x][y][z].r = color.r;
    g_rgb_buff[x][y][z].g = color.g;
    g_rgb_buff[x][y][z].b = color.b;
}

void Alter_Voxel(int x, int y, int z, int state) {
    if (state == 1)
        Voxel_Set_RGB(x, y, z, Voxel_Get_RGB(x, y, z));
    else
        Voxel_Clr(x, y, z);
}

RGB Voxel_Get_RGB(uint8 x, uint8 y, uint8 z) {
    RGB rgb;
    rgb.r = g_rgb_buff[x][y][z].r;
    rgb.g = g_rgb_buff[x][y][z].g;
    rgb.b = g_rgb_buff[x][y][z].b;

    return rgb;
}

uint8 Voxel_Get_State(uint8 x, uint8 y, uint8 z) {
    if (g_rgb_buff[x][y][z].r == 0
            && g_rgb_buff[x][y][z].g == 0
            && g_rgb_buff[x][y][z].b == 0)
        return 0;
    else
        return 1;
}


// Set a single voxel to OFF
void Voxel_Clr(uint8 x, uint8 y, uint8 z) {
	//	if (In_Range(x,y,z))
    g_rgb_buff[x][y][z].r = 0;
    g_rgb_buff[x][y][z].g = 0;
    g_rgb_buff[x][y][z].b = 0;
}

void Fill_Plane_X(uint8 X) {
    uint8 y, z;
    if (X >= 0 && X < X_MAX) {
        for (y = 0; y < Y_MAX; z++) {
            for (z = 0; z < Z_MAX; y++) {
                Voxel_Set_RGB(X, y, z, Get_Color_RGB());
            }
        }
    }
}

void Clear_Plane_X(uint8 X) {
    uint8 y, z;
        for (y = 0; y <= Y_MAX; y++) {
            for (z = 0; z <= Z_MAX; y++) {
                Voxel_Clr(X, y, z);
            }
        }
 }

void Fill_Plane_Y(uint8 Y) {
    uint8 x, z;
    if (Y >= 0 && Y < Y_MAX) {
        for (x = 0; x < X_MAX; x++) {
            for (z = 0; z < Z_MAX; z++) {
                Voxel_Set_RGB(x, Y, z, Get_Color_RGB());
            }
        }
    }
}

void Clear_Plane_Y(uint8 Y) {
    uint8 x, z;

    for (x = 0; x <= X_MAX; x++) {
        for (z = 0; z <= Z_MAX; z++) {
            Voxel_Clr(x, Y, z);
        }
    }
 }

void Fill_Plane_Z(uint8 Z) {
    uint8 x, y;

    if (Z >= 0 && Z < Z_MAX) {
        for (x = 0; x < X_MAX; x++) {
            for (y = 0; y < Y_MAX; y++) {
                Voxel_Set_RGB(x, y, Z, Get_Color_RGB());
            }
        }
    }
}

void Clear_Plane_Z(uint8 Z) {
    uint8 x, y;

    for (x = 0; x <= X_MAX; x++) {
        for (y = 0; y <= Y_MAX; y++) {
            Voxel_Clr(x, y, Z);
        }
    }
}

// Shift the entire cube framebuffer up, down, left, or right one axis increment
// Working, could stand to be optimized
void Shift(char axis, SHIFT_DIR dir) {
    uint8 x, y, z;
    RGB rgb[8][8][8];
    // load temp buffer with current frame of cube output buffer
    memcpy(rgb, g_cube_fb, sizeof(g_cube_fb));

    switch (axis) {
        case ('X') :
            if (dir == BWD ) {
                for (x = X_MIN; x < X_MAX; x++) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x+1][y][z] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
            FB_Update();
            Clear_Plane_X(X_MIN);
            }

            if (dir == FWD ) {
                for (x = X_MAX; x > X_MIN; x--) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x-1][y][z] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
                FB_Update();
                Clear_Plane_X(X_MAX);
            }
            break;      // Case 'X'

        case ('Y') :
            if (dir == LEFT ) {
                for (y = Y_MIN; y < Y_MAX; y++) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x][y+1][z] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
                FB_Update();
                Clear_Plane_Y(Y_MIN);
            }

            if (dir == RIGHT ) {
                for (y = Y_MAX; y > Y_MIN; y--) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x][y-1][z] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
                FB_Update();
                Clear_Plane_Y(Y_MAX);
            }
            break;      // Case 'Y'

       case ('Z') :
            if (dir == UP ) {
                for (z = Z_MIN; z < Z_MAX; z++) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (x = 0; x <= X_MAX; x++) {
                            g_rgb_buff[x][y][z+1] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
                FB_Update();
                Clear_Plane_Z(Z_MIN);
            }

            if (dir == DOWN ) {
                for (z = Z_MAX; z > 0; z--) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (x = 0; x <= X_MAX; x++) {
                            g_rgb_buff[x][y][z-1] = rgb[x][y][z];    // replace all voxels on this Z-axis with the axis above (Z+1)

                        }
                    }
                }
                FB_Update();
                Clear_Plane_Z(Z_MAX);
            }
            break;      // Case 'Z'

        default:
            break;
    } // switch
}

 // Clears the RGB frame buffer
void Clear_Frame() {
    memset(g_rgb_buff, 0, sizeof(g_rgb_buff));
 }

// Updates the cube output framebuffer  with data from the current framebuffer
// This prevents needing to clear out the entire framebuffer in between frame
// draws which can cause flicker at lower frame rates
void FB_Update() {
    uint8 x, y, z;
    // Disable interrupts while updating frame buffer
    // *** This blocks for 512 friggin instructions to complete the loop. Surely there's a better way???
    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
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
    
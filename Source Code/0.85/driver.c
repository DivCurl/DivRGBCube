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

// hmmm.....WIP
void Draw_Line(uint8 x1, uint8 x2, uint8 y1, uint8 y2, uint8 z1, uint8 z2) {

}


void Fill_Plane_X(uint8 X) {
    uint8 y, z;
    if (X >= 0 && X < X_MAX) {
        for (y = 0; y < Y_MAX; z++) {
            for (z = 0; z < Z_MAX; y++) {
                Voxel_Set_RGB(X, y, z, Get_Color_RGB(X, y, z));
            }
        }
    }
}

void Clear_Plane_X(uint8 X) {
    uint8 y, z;
        for (y = 0; y <= Y_MAX; y++) {
            for (z = 0; z <= Z_MAX; z++) {
                Voxel_Clr(X, y, z);
            }
        }
 }

void Fill_Plane_Y(uint8 Y) {
    uint8 x, z;
    if (Y >= 0 && Y < Y_MAX) {
        for (x = 0; x < X_MAX; x++) {
            for (z = 0; z < Z_MAX; z++) {
                Voxel_Set_RGB(x, Y, z, Get_Color_RGB(x, Y, z));
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
                Voxel_Set_RGB(x, y, Z, Get_Color_RGB(x, y, Z));
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
// Working, could stand to be optimized. Looks kinda dirty.
void Shift(char axis, SHIFT_DIR dir) {
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
                FB_Update();
                Clear_Plane_X(X_MIN);
            }

            if (dir == LEFT ) {
                for (x = X_MAX; x > X_MIN; x--) {
                    for (y = 0; y <= Y_MAX; y++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x-1][y][z] = rgb[x][y][z];  

                        }
                    }
                }
                FB_Update();
                Clear_Plane_X(X_MAX);
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
                FB_Update();
                Clear_Plane_Y(Y_MIN);
            }

            if (dir == IN ) {
                for (y = Y_MAX; y > Y_MIN; y--) {
                    for (x = 0; x <= X_MAX; x++) {
                        for (z = 0; z <= Z_MAX; z++) {
                            g_rgb_buff[x][y-1][z] = rgb[x][y][z];

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
                            g_rgb_buff[x][y][z+1] = rgb[x][y][z]; 

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
                            g_rgb_buff[x][y][z-1] = rgb[x][y][z];   

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
    
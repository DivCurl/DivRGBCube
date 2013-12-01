/* Low-level drawing and helper functions to assist in high level tasks and animation functions */

// #include "main.h"
#include "driver.h"
#include "main.h"

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
void VoxelSetSolidRGB(uint8 x, uint8 y, uint8 z, SOLID_RGB color) {
    
    // if (In_Range(x,y,z))
    // First clear the data in the buffer for this voxel
  // VoxelClrRGB(x, y, z);
    // Set the voxel to the appropriate solid R, G, or B color
    switch (color) {
        case (RED) :
            cube_fb[x][y][z].r = 255;
            cube_fb[x][y][z].g = 0;
            cube_fb[x][y][z].b = 0;
            break;
        case (ORANGE) :
            cube_fb[x][y][z].r = 255;
            cube_fb[x][y][z].g = 125;
            cube_fb[x][y][z].b = 0;
            break;
        case (YELLOW) :
            cube_fb[x][y][z].r = 255;
            cube_fb[x][y][z].g = 255;
            cube_fb[x][y][z].b = 0;
            break;
        case (GREEN) :
            cube_fb[x][y][z].r = 0;
            cube_fb[x][y][z].g = 255;
            cube_fb[x][y][z].b = 0;
            break;
        case (CYAN) :
            cube_fb[x][y][z].r = 0;
            cube_fb[x][y][z].g = 255;
            cube_fb[x][y][z].b = 255;
            break;
        case (BLUE) :
            cube_fb[x][y][z].r = 0;
            cube_fb[x][y][z].g = 0;
            cube_fb[x][y][z].b = 255;
            break;
        case (VIOLET) :
            cube_fb[x][y][z].r = 125;
            cube_fb[x][y][z].g = 0;
            cube_fb[x][y][z].b = 255;
            break; 
        case (MAGENTA) :
            cube_fb[x][y][z].r = 255;
            cube_fb[x][y][z].g = 0;
            cube_fb[x][y][z].b = 255;
            break;

        default:
            break;
    }
}

void VoxelSetRGB(uint8 x, uint8 y, uint8 z, RGB color) {
    cube_fb[x][y][z].r = color.r;
    cube_fb[x][y][z].g = color.g;
    cube_fb[x][y][z].b = color.b;
}


// Set a single voxel to OFF
void VoxelClr(uint8 x, uint8 y, uint8 z) {
	//	if (In_Range(x,y,z))
    cube_fb[x][y][z].r = 0;
    cube_fb[x][y][z].g = 0;
    cube_fb[x][y][z].b = 0;
}

void ClrCube() {
    uint8 x, y, z;

    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
                cube_fb[x][y][z].r = 0;
                cube_fb[x][y][z].g = 0;
                cube_fb[x][y][z].b = 0;
            }
        }
    }
}
    
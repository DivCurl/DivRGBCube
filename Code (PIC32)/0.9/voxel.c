#include "voxel.h"
/* Some basic voxel drawing functions */

// Assigns the input RGB color to the specified voxel
void VXSet(uint8 x, uint8 y, uint8 z, RGB color) {
    g_rgb_buff[x][y][z].r = color.r;
    g_rgb_buff[x][y][z].g = color.g;
    g_rgb_buff[x][y][z].b = color.b;
}

// Sets a single voxel to a solid R, G, or B color
void VXSetSolid(uint8 x, uint8 y, uint8 z, SOLID_RGB color) {

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

// Blanks the specified voxel
void VXClr(uint8 x, uint8 y, uint8 z) {
    g_rgb_buff[x][y][z].r = 0;
    g_rgb_buff[x][y][z].g = 0;
    g_rgb_buff[x][y][z].b = 0;
}

/* WIP - Tested and working currently for X-Y plane only
 * Sends a voxel 'flying' down a specified axis given a 'start' and 'end' cooridnate. The voxel stays lit
 * once the 'end' position is reached.  Function designed to limit the number of input parameters.
 * Needs some cleaning up
 */
void VXFly(const char *plane, int16 str, int16 end, int16 offs_ortho, int16 offs_perp, uint16 delay) {
    int16 i;

    if (!(strcmp(plane, "XY"))) {
        for (i = str; i <= end; i++) {
                VXSet(offs_ortho, i, offs_perp, RGBGetColor(offs_ortho, i, offs_perp));
                if ((i-1) >= 0) // we don't want to go outside the buffer
                        VXClr(offs_ortho, (i-1), offs_perp);   // Clear the previous Voxel before updating the current one
                DRVFBUpdate();
                while (!(DRVDelayms(delay))) {}   // short delay to allow each voxel in sequence to be shown before next one is lit
        }
    }

    if (!(strcmp(plane, "XZ"))) {


    }

    if (!(strcmp(plane, "YZ"))) {

        
    }
 }

// Returns the RGB color assigned to this voxel. Not sure if this will be useful yet.
RGB VXGetRGB(uint8 x, uint8 y, uint8 z) {
    RGB rgb;
    rgb.r = g_rgb_buff[x][y][z].r;
    rgb.g = g_rgb_buff[x][y][z].g;
    rgb.b = g_rgb_buff[x][y][z].b;

    return rgb;
}

// Returns state of this voxel (on or off)
uint8 VXState(uint8 x, uint8 y, uint8 z) {
    if (g_rgb_buff[x][y][z].r == 0
            && g_rgb_buff[x][y][z].g == 0
            && g_rgb_buff[x][y][z].b == 0)
        return 0;
    else
        return 1;
}

// Swaps the the two target voxels in the cube buffer
// NOTE: his will also swap the color info along with the state
// What if we want the color info to remain untouched???
void VXSwap(uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2) {
    RGB tmp1, tmp2;
    tmp1 = g_rgb_buff[x1][y1][z1];
    tmp2 = g_rgb_buff[x2][y2][z2];
    g_rgb_buff[x1][y1][z1] = tmp2;
    g_rgb_buff[x2][y2][z2] = tmp1;
}

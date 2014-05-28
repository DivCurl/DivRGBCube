/******************************************************************************
 * Some basic lower-level voxel drawing and manipulation functions
 *
 * Written by Mike Dillmann
 ******************************************************************************/
#include "../include/voxel.h"

// Assigns the input RGB color to the specified voxel
void V_Set(uint8 x, uint8 y, uint8 z, rgb_t color) {

    if ( TESTFLAG ( gblOpts.AFLAGS, ANIM_USE_GLOBAL ) ) {
        if ( gblOpts.rgbMode == RGB_MODE_SOLID )  {
            g_colorBuffer[x][y][z] = gblOpts.color;
        }

        if ( gblOpts.rgbMode == RGB_MODE_FADE ) {
            g_colorBuffer[x][y][z] = R_GetAngleColor( ptrAnim->rgbOpts.angle );
        }

        if ( gblOpts.rgbMode == RGB_MODE_DEF ) {
            g_colorBuffer[x][y][z] = color;
        }
    } else {
        g_colorBuffer[x][y][z] = color;
    }
}

// Blanks the specified voxel at the specified input coordinates
void V_Clr(uint8 x, uint8 y, uint8 z) {
    // using color_negative, we can specify a 'blank' color other
    // than OFF (0)
    g_colorBuffer[x][y][z].r = 0;
    g_colorBuffer[x][y][z].g = 0;
    g_colorBuffer[x][y][z].b = 0;
}

/* WIP - Tested and working currently for X-Y plane only
 * Sends a voxel 'flying' down a specified axis given a 'start' and 'end' cooridnate. The voxel stays lit
 * once the 'end' position is reached.  Function designed to limit the number of input parameters.
 */
void V_Fly(const char *plane, int16 str, int16 end, int16 offs_ortho, int16 offs_perp, uint16 delay) {
    int16 i;

    if (!(strcmp(plane, "XY"))) {
        for (i = str; i <= end; i++) {
            V_Set(offs_ortho, i, offs_perp, R_GetAngleColor( ptrAnim->rgbOpts.angle ) );
            if ((i-1) >= 0) // we don't want to go outside the buffer
                    V_Clr(offs_ortho, (i-1), offs_perp);   // Clear the previous Voxel before updating the current one
            D_FBUpdate();
            while ( !D_Delayms( frmCtr.pre ) ) {}   // short delay to allow each voxel in sequence to be shown before next one is lit
        }
    }

    if (!(strcmp(plane, "XZ"))) {


    }

    if (!(strcmp(plane, "YZ"))) {

        
    }
 }

// Returns the RGB color assigned to this voxel.
// Not sure if this will be useful yet.
rgb_t V_GetRGB(uint8 x, uint8 y, uint8 z) {

    return g_colorBuffer[x][y][z];
}

// Returns state of this voxel (on or off)
bool V_State(uint8 x, uint8 y, uint8 z) {
    if (g_colorBuffer[x][y][z].r == 0
            && g_colorBuffer[x][y][z].g == 0
            && g_colorBuffer[x][y][z].b == 0)
        return 0;
    else
        return 1;
}

// Swaps the the two target voxels in the cube buffer
// NOTE: his will also swap the color info along with the state
// What if we want the color info to remain untouched???
void V_Swap(uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2) {
    rgb_t tmp1, tmp2;
    tmp1 = g_colorBuffer[x1][y1][z1];
    tmp2 = g_colorBuffer[x2][y2][z2];
    g_colorBuffer[x1][y1][z1] = tmp2;
    g_colorBuffer[x2][y2][z2] = tmp1;
}

// Swaps the specified voxel position but clears the initial
void V_Mov( uint8 x1, uint8 y1, uint8 z1, uint8 x2, uint8 y2, uint8 z2 ) {
    g_colorBuffer[x2][y2][z2] = g_colorBuffer[x1][y1][z1];
    V_Clr( x1, y1, z1 );
}

void V_Flip(uint8 x, uint8 y, uint8 z, bool state, rgb_t color) {
    if (state == 1) {
        V_Set(x, y, z, color);
    } else {
        V_Clr(x, y, z);
    }
}
#include "rgb.h"
/* Functions for filling the cube color buffer */

// Function handler for various color modes
void RGBRefreshColorBuffer(void) {
    
    switch (g_rgb_opts.fade_mode) {
        case (FADE_CW_CONT) :
            RGBFadeCWCont();
            break;
        case (FADE_CW_INPUT) :
    //         Fade_CW_Input();
            break;
        case (FADE_PREBAKED) :
            RGBFadePrebaked();
            break;
        // Sets the entire color buffer to a solid color
        case (SOLID_COLOR) :
            RGBFadeSolid();
            break;
        default:
            break;
    }
}

RGB RGBGetColor(uint8 x, uint8 y, uint8 z) {

    return (g_color_buff[x][y][z]);   //  The color that gets assigned to the current voxel being processsed
}

// Working, but needs some tweaking
// TODO: FADE DELAY DOESN'T YET CURRENTLY WORK PROPERLY. COMMENTED OUT FOR TESTING. 
void RGBFadeCWCont() {

    static RGB color;
    static int16 angle, tmp, count, dummy;
    uint8 x, y, z;

    // init starting color to red
    // TODO - MAKE THIS ADJUSTABLE?
    if (color.r == 0 && color.g == 0 && color.b == 0) {
    color.r = 255;
    color.g = 0;
    color.b = 0;
    }

/*
    if (g_ms_ctr != tmp) {
        tmp = g_ms_ctr;
        count++;
    }

    if (count == g_rgb_opts.fade_delay) {
        count = 0;
   
*/
                
    if ((angle < 0) || (angle >= 360)) {
        angle = 0;
        color.r = 255;
        color.g = 0;
        color.b = 0;
    }

    if ((angle == 0)) {         //  RED -> YELLOW
        color.r == 255;
        color.g += g_rgb_opts.angle_roc;
        if (color.g >= 255) {
            color.r = 255;
            color.g = 255;
            color.b = 0;
            angle = 60;
        }
    }

    if ((angle == 60)) {        // YELLOW > GREEN
        color.r -= g_rgb_opts.angle_roc;
       if ((color.r <= 0) && (color.g == 255)) {
           color.r = 0;
           color.g = 255;
           color.b = 0;
           angle = 120;
        }
    }

    if ((angle == 120)) {       // GREEN -> CYAN
        color.b += g_rgb_opts.angle_roc;
        if ((color.g == 255) && (color.b >= 255)) {
            color.r = 0;
            color.g = 255;
            color.b = 255;
            angle = 180;
        }
    }

   if ((angle == 180)) {        // CYAN -> BLUE
        color.g -= g_rgb_opts.angle_roc;
        if ((color.g <= 0) && (color.b == 255)) {
            color.r = 0;
            color.g = 0;
            color.b = 255;
            angle = 240;
        }
   }

   if ((angle == 240)) {        // BLUE -> MAGENTA
        color.r += g_rgb_opts.angle_roc;
        if ((color.r >= 255) && (color.b == 255)) {
            color.r = 255;
            color.g = 0;
            color.b = 255;
            angle = 300;
        }
   }

    if ((angle == 300)) {        // MAGENTA -> RED
        color.b -= g_rgb_opts.angle_roc;
        if ((color.r == 255) && (color.b <= 0)) {
            // Reset back to zero degrees
            color.r = 255;
            color.g = 0;
            color.b = 0;
            g_rgb_color.b  = 0;
            angle = 0;
        }
    }

    // Update Color Buffer
    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
                g_color_buff[x][y][z] = color;
            }
        }
    }
// }    COMMENTED OUT FOR TESTING
}

// Set entire cube to preset solid color.
// Might remove need for RGBSetSolid() -- it's becoming kind of cumbersome to have
// two different 'put voxel' functions
void RGBFadeSolid() {
    RGB color;
    uint8 x, y, z;

    switch (g_rgb_opts.opt1) {
        case (RED) :
            color.r = 255;
            color.g = 0;
            color.b = 0;
            break;
        case (ORANGE) :
            color.r = 255;
            color.g = 125;
            color.b = 0;
            break;
        case (YELLOW) :
            color.r = 255;
            color.g = 255;
            color.b = 0;
            break;
        case (GREEN) :
            color.r = 0;
            color.g = 255;
            color.b = 0;
            break;
        case (CYAN) :
            color.r = 0;
            color.g = 255;
            color.b = 255;
            break;
        case (BLUE) :
            color.r = 0;
            color.g = 0;
            color.b = 255;
            break;
        case (VIOLET) :
            color.r = 125;
            color.g = 0;
            color.b = 255;
            break;
        case (MAGENTA) :
            color.r = 255;
            color.g = 0;
            color.b = 255;
            break;
        default:
            break;
    }

     // Update Color Buffer
    for (z = 0; z <= Z_MAX; z++) {
        for (y = 0; y <= Y_MAX; y++) {
            for (x = 0; x <= X_MAX; x++) {
                g_color_buff[x][y][z] = color;
            }
        }
    }
}

/*
 * New idea:
 * Wipes entire cube from one color to another, starting e.g. at lower-left
 * and wiping to upper-right with a delay between each transition as the wipe
 * moves over the cube.
 *
 */
void RGBFadeWipe(void) {

}

// This function prerenders all colors for the entire cube space.
// WIP, not currently working, don't use yet
void RGBPrebakeColors() {
    uint16 clr_delta_1, clr_delta_2, m;
    uint8 x, y, z, i, j, k;
    clr_delta_1 = DRVRound((float)(255/8));
    RGB prev_color;

    prev_color.r = 255;
    prev_color.g = 0;
    prev_color.b = 0;

    //      prev_color.r -= 31;
    prev_color.g -= 0;
    prev_color.b += 100;

    for (k = 0; k <= 7; k++) {
        for (j = 0; j <= 7; j++) {
            for (i = 0; i <= 7; i++) {

            }

        }
        
    }
}

// This function will assign color values to each LED according to a prebaked RGB buffer
// making up the space for an entire cube.
// IS THIS REALLY NEEDED ANYMORE?
void RGBFadePrebaked() {
    g_rgb_color.r = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].r;
    g_rgb_color.g = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].g;
    g_rgb_color.b = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].b;
}
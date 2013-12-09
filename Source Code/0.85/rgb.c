#include "rgb.h"
/* Functions for filling the cube color buffer */

// Function handler for various color modes
void Refresh_Color_Buffer(void) {
    
    switch (g_rgb_opts.fade_mode) {
        case (FADE_CW_CONT) :
            Fade_CW_Cont();
            break;
        case (FADE_CW_INPUT) :
    //         Fade_CW_Input();
            break;
        case (FADE_PREBAKED) :
            Fade_Prebaked();
            break;
        case (SOLID_COLOR) :
            break;
        default:
            break;
    }
}

RGB Get_Color_RGB(uint8 x, uint8 y, uint8 z) {

    return (g_color_buff[x][y][z]);   //  The color that gets assigned to the current voxel being processsed
}

// Working, but needs some tweaking
// TODO: FADE DELAY DOESN'T YET CURRENTLY WORK PROPERLY. COMMENTED OUT FOR TESTING. 
void Fade_CW_Cont() {

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

// This function prerenders all colors for the entire cube space.
// WIP, not currently working, don't use yet
void Prebake_Cube_Colors() {
    uint16 clr_delta_1, clr_delta_2, m;
    uint8 x, y, z, i, j, k;
    clr_delta_1 = Round((float)(255/8));
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
void Fade_Prebaked() {
    g_rgb_color.r = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].r;
    g_rgb_color.g = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].g;
    g_rgb_color.b = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].b;
}
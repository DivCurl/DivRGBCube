#include "rgb.h"
/* Functions for filling the cube buffer with color data */

// Color fading mode selection handler
// Allows setting voxel RGB colors based on various color fade modes
RGB Get_Color_RGB() {
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
    return (g_rgb_color);   //  The color that gets assigned to the current voxel being processsed
}

// Working, but needs some tweaking
void Fade_CW_Cont() {
    // Increase the RGB angle in increments.
    // 0 Degrees = RED (255,0,0)
    // 120 degress = GREEN (0,255,0)
    // 240 degrees = BLUE (0,0,255)

    static uint16 angle = 0;
    static uint16 count, tmp = 0;
     
    if ((angle < 0) || (angle >= 360)) {
        angle = 0;
        g_rgb_color.r = 255;
        g_rgb_color.g = 0;
        g_rgb_color.b = 0;
    }

    if (g_ms_ctr != tmp) {
        tmp = g_ms_ctr;
        count++;
    }

    if (count == g_rgb_opts.fade_delay) {
        count = 0;

        if ((angle == 0)) {         //  RED -> YELLOW
            g_rgb_color.r == 255;
            g_rgb_color.g += g_rgb_opts.angle_roc;
            if (g_rgb_color.g >= 255) {
                g_rgb_color.g = 255;
                angle = 60;
            }
        }
                    
        if ((angle == 60)) {        // YELLOW > GREEN
            g_rgb_color.r -= g_rgb_opts.angle_roc;
           if ((g_rgb_color.r <= 0) && (g_rgb_color.g == 255)) {
               g_rgb_color.r = 0;
               angle = 120;
            }
        }

        if ((angle == 120)) {       // GREEN -> CYAN
            g_rgb_color.b += g_rgb_opts.angle_roc;
            if ((g_rgb_color.g == 255) && (g_rgb_color.b >= 255)) {
                g_rgb_color.b = 255;
                angle = 180;
            }
        }

       if ((angle == 180)) {        // CYAN -> BLUE
            g_rgb_color.g -= g_rgb_opts.angle_roc;
            if ((g_rgb_color.g <= 0) && (g_rgb_color.b == 255)) {
                g_rgb_color.g = 0;
                angle = 240;
            }
       }

       if ((angle == 240)) {        // BLUE -> MAGENTA
            g_rgb_color.r += g_rgb_opts.angle_roc;
            if ((g_rgb_color.r >= 255) && (g_rgb_color.b == 255)) {
                g_rgb_color.r = 255;
                angle = 300;
            }
       }
        
        if ((angle == 300)) {        // MAGENTA -> RED
            g_rgb_color.b -= g_rgb_opts.angle_roc;
            if ((g_rgb_color.r == 255) && (g_rgb_color.b <= 0)) {
                // Reset back to zero degrees
                g_rgb_color.r  = 255;
                g_rgb_color.g  = 0;
                g_rgb_color.b  = 0;
                angle = 0;
            }
       }
    }
}

// This function prerenders all colors for the entire cube space.
void Prebake_Cube_Colors() {

}

// This function will assign color values to each LED according to a prebaked RGB buffer
// making up the space for an entire cube.
void Fade_Prebaked() {
    g_rgb_color.r = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].r;
    g_rgb_color.g = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].g;
    g_rgb_color.b = g_cube_prebaked[g_rgb_opts.x][g_rgb_opts.y][g_rgb_opts.z].b;
}
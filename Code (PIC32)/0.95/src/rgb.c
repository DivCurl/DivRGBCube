/******************************************************************************
 * RGB color space generation and manipulation functions
 * Design note: there are three (3) total buffers which occupy the total
 * cube space --
 *
 * 1. g_cube_fb - this is the final output 'framebuffer' and holds the current
 * output frame which is processed by the main display refresh interrupt (Timer2)
 *
 * 2. g_rgb_buff - this is a pre-output framebuffer which holds the current
 * pending frame to be sent to the main framebuffer. This second (double?)
 * buffer is what the animation sequences operate on directly and is pushed to
 * the output via DRVFBUpdate() in driver.c
 *
 * 3. g_color_buff - this buffer exlusively contains color data for the entire
 * cube display. Every element in this buffer is assigned a color, which
 * is then 'picked' out by the animation sequences being run. This buffer is
 * updated at a specified rate via RGBRefreshColorBuffer(), which is called
 * via a periodic timer interrupt (Timer4)
 *
 * Written by Mike Dillmann
 *
 ******************************************************************************/
#include "../include/rgb.h"
/* Functions for filling the cube color buffer */

// Function handler for various color modes
rgb_t *g_colors;
extern rgb_t g_colorBuffer[8][8][8];
void R_RefreshColorBuffer(uint8 mode) {
    
    switch (mode) {
        case (FADE_CW_CONT) :
//            RGBFadeCWCont();
            break;
        case (FADE_CW_INPUT) :
    //         Fade_CW_Input();
            break;
        // Sets the entire color buffer to a solid color
        case (FADE_SOLID) :
//            RGBFadeSolid();
            break;
        case (FADE_WIPE) :
            R_FadeWipe();
            break;
        default:
            break;
    }
}
rgb_t R_GetNewColor() {
    rgb_t col;
    return col;
}

void R_GetRandomDelay() {
//    g_rgb_opts.delay = rand() % 750; // 750 is about the slowest we ever weant to go
}

void R_GetRandomFadeDelay() {
//    g_rgb_opts.fade_delay = rand() % 500;
}

void R_GetRandomFadeROC() {
//    g_rgb_opts.angle_roc = rand() % 15;
}

void R_GetRandomFadeMode() {
//    g_rgb_opts.fade_mode = rand() % 6;
}

rgb_t R_GetAngleColor(float theta) {        // limit to 0.25 degree input resolution for simplicty
    uint16 i;
    i = (theta / 360 * (COLOR_ARRAY_LEN - 1));

    return g_colors[i];
}

// Returns a single RGB color to the calling function.
rgb_t R_GetRandomColor() {
    rgb_t rcolor;
    // generate random values bewteen 0 and
    // Note: may want to add a weighting factor and/or limit the
    // values to prevent production of white color to limit the current
    // demand of the common anode bus
    uint8 random_R = rand() % 255;
    uint8 random_G = rand() % 255;
    uint8 random_B = rand() % 255;
    rcolor.r = random_R;
    rcolor.g = random_G;
    rcolor.b = random_B;
    
    return rcolor;
}

rgb_t R_GetColorCoord(uint8 x, uint8 y, uint8 z) {
    return (g_colorBuffer[x][y][z]);   //  The color that gets assigned to the current voxel being processsed
}

rgb_t R_SetColorRGB(int r, int g, int b) {
    rgb_t color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

// Checks equality of voxel colors
bool R_ColorEqual(rgb_t clr1, rgb_t clr2) {
    if ((clr1.r == clr2.r) && (clr1.g == clr2.g) && (clr1.b == clr2.b))
        return 1;
    else
        return 0;
}

// Initialize color wheel for smooth-fading of pre-fetched RGB values
void R_FillColorVector() {
    // get color array length based on max bcm bit - this determines the color resolution
    int i;
//    const int colArrayLength =

    // RGB color wheel: 0-360 deg
    // 256 colors / 60 deg (0 (off) color inclusive
    // 360 / 60 deg = 6 * 256 = 1536 total RGB elements in array
    // degree/color resolution: 360 deg / 1536 colors = 0.234

    // 0deg = 255-0-0 (red)        -> element 0-255
    // 60deg = 255-255-0 (yellow)  -> element 256-511
    // 120deg = 0-255-0 (green)    -> element 512-767
    // 180deg = 0-255-255 (cyan)   -> element 768-1023
    // 240deg = 0-0-255 (blue)     -> element 1024-1279
    // 300deg = 255-0-255 (mag)    -> element 1280-1535
    
    g_colors = (rgb_t*)malloc(COLOR_ARRAY_LEN);   // put color array on the heap
    for (i = 0; i < COLOR_ARRAY_LEN; i++ ) {

        // 0 deg to 60 deg (red to yellow)
        if (i <= 255) {
            g_colors[i].r = (255) >> CUBE_BCM_RES;
            g_colors[i].g = (i) >> CUBE_BCM_RES;
            g_colors[i].b = 0;
        }

        // 60 deg to 120 deg (yellow to green)
        if (i > 255 && i <= 511) {
            g_colors[i].r = (511-i) >> CUBE_BCM_RES;
            g_colors[i].g = (255) >> CUBE_BCM_RES;
            g_colors[i].b = 0;
        }

        // 120 deg to 180 deg (green to cyan)
        if (i > 511 && i <= 767) {
            g_colors[i].r = 0;
            g_colors[i].g = (255) >> CUBE_BCM_RES;
            g_colors[i].b = (i-512) >> CUBE_BCM_RES;
        }

        // 180 deg to 240 deg (cyan to blue)
        if (i > 767 && i <= 1023) {
            g_colors[i].r = 0;
            g_colors[i].g = (1023-i) >> CUBE_BCM_RES;
            g_colors[i].b = 255 >> CUBE_BCM_RES;
        }

        // 240 deg to 300 deg (blue to magenta)
        if (i > 1023 && i <= 1279) {
            g_colors[i].r = (i-1024) >> CUBE_BCM_RES;
            g_colors[i].g = 0;
            g_colors[i].b = 255 >> CUBE_BCM_RES;
        }

        // 300 deg to 360 deg (magenta to red)
        if (i > 1279 && i <= 1535) {
            g_colors[i].r = 255 >> CUBE_BCM_RES;
            g_colors[i].g = 0;
            g_colors[i].b = (1535-i) >> CUBE_BCM_RES;
        }
    }   // outer for loop
     
}

/*
 *  Partially working. WIP. 
 *  Seems to be a bound checking problem?
 */
void R_FadeWipe(void) {
    /* 1. Start at front-lower-left corner of cube (0,0,0).
     * 2. Replace voxel with new color.
     * 3. wait a short while before moving to next voxel.
     * 4. Continue until we reach rear-upper-right corner of cube (7,7,7)
     */
     // Testing with solid color wipe first.
    rgb_t color;
    uint16 i, j, k;

    color.r = 255;
    color.g = 0;
    color.b = 0;

    static uint8 x, y, z;

    // Fade front-back
    if ( TESTFLAG( ptrAnim->rgbOpts.FLAGS, RGB_FADE_MODE_CHANGED ) ) {
        if ( x <= CUBE_X_MAX ) {
             
            for ( i = 0; i <= 7; i++ ) {
                for ( j = 0; j <= 7; j++ )
                    g_colorBuffer[x][i][j] = color;
            }
            x++;
        }

        else {
            x = 0;
            y = 0;
            z = 0;
        }
    }

    else {
            x = 0;
            y = 0;
            z = 0;
    }
}

void R_CubeSetSolid(rgb_t color) {
uint8 x, y, z;
    for (z = 0; z <= CUBE_Z_MAX; z++) {
        for (y = 0; y <= CUBE_Y_MAX; y++) {
            for (x = 0; x <= CUBE_X_MAX; x++) {
                V_Set( x, y, z, color );
            }
        }
    }
}

void R_SetNegative(rgb_t* cube_neg_color, rgb_t new_color) {
    *cube_neg_color = new_color;
}
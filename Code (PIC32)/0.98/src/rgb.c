#include "../include/rgb.h"
/* Functions for filling the cube color buffer */

// Function handler for various color modes
rgb_t *g_colors;
extern rgb_t g_colorBuffer[8][8][8];
/*
===================
 Name

 Desc
===================
*/
void R_RefreshColorBuffer( uint8 mode ) {
    
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


/*
===================
 Name

 Desc
===================
*/
rgb_t R_GetAngleColor( float angle ) {        // limit to 0.25 degree input resolution for simplicty
    uint16 i;
    if ( angle >= 360 ) {
        angle -= 360;
    }
    i = ( angle / 360 * ( COLOR_ARRAY_LEN - 2 ) );

    return g_colors[i];
}

/*
===================
 Name

 Desc
===================
*/
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

/*
===================
 Name

 Desc
===================
*/
rgb_t R_SetColorRGB(int r, int g, int b) {
    rgb_t color;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

/*
===================
 Name

 Desc
===================
*/
bool R_ColorEqual(rgb_t clr1, rgb_t clr2) {
    if ((clr1.r == clr2.r) && (clr1.g == clr2.g) && (clr1.b == clr2.b))
        return 1;
    else
        return 0;
}

/*
===================
 Name

 Desc
===================
*/
void R_FillColorVector() {
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
    
    // put color array on the heap
    g_colors = (rgb_t*)malloc(COLOR_ARRAY_LEN);
    /*
     * Note: the bit shifts below automatically scale the
     * color values in the buffer according to the bcm resolution
     * being used. E.g. for a bcm resolution of 4 bits,
     * a solid RED color (255,0,0) becomes (15,0,0) after
     * shifting. This allows the buffer to be filled with the appropriate
     * RGB values regardless of the bcm resolution being used.
     */
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
    }     
}

/*
 *  Partially working. WIP. 
 *  Seems to be a bound checking problem?
 */
/*
===================
 Name

 Desc
===================
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
    /*
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
     */
}

/*
===================
 Name

 Desc
===================
*/
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

/*
===================
 Name

 Desc
===================
*/
void R_SetNegative(rgb_t* cube_neg_color, rgb_t new_color) {
    *cube_neg_color = new_color;
}
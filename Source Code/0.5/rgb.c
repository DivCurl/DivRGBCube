#include "rgb.h"
/* Functions for filling the cube buffer with color data */


// !!! THIS FUNCTION IS CURRENTLY BROKEN !!!
void FillColor() {
    // Increase the RGB angle in increments.
    // 0 Degrees = RED (255,0,0)
    // 120 degress = GREEN (0,255,0)
    // 240 degrees = BLUE (0,0,255)

static uint16 angle = 0;
static uint16 count = 0;
        count++;
        // Testing---doesn't work.
        if ((angle < 0) || (angle >= 360)) {
            angle = 0;
            g_rgb_color.r = 255;
            g_rgb_color.g = 0;
            g_rgb_color.b = 0;   
        }
/*
        if (count == 500) {
            count = 0;

        if ((angle == 0)) {
     //       if ((g_rgb_color.r == 255) && (g_rgb_color.g == 255))

     //           angle = 60;
     //      else
                g_rgb_color.b++;
        }
            

        if ((angle == 60)) {
           if ((g_rgb_color.r == 0) && (g_rgb_color.g == 255))
               angle = 120;
           else
            g_rgb_color.r--;
        }

        if ((angle == 120)) {
            if ((g_rgb_color.g == 255) && (g_rgb_color.b == 255))
                angle = 180;
            else
            g_rgb_color.b++;
        }

       if ((angle == 180)) {
            if ((g_rgb_color.g == 0) && (g_rgb_color.b == 255))
                angle = 240;
            else
            g_rgb_color.g--;
       }

       if ((angle == 240)) {
            if ((g_rgb_color.r == 255) && (g_rgb_color.b == 0))
                angle = 0;
            else
            g_rgb_color.r++;
       }
 */


}
#ifndef MAIN_H
#define	MAIN_H

#include <p32xxxx.h>    // PIC32 specifics header file.
#include <plib.h>       // PIC32 Peripheral Library.
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "defs.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

// GLOBALS
volatile uint16 g_ms_ctr;       // millisecond counter
volatile uint16 g_fade_ctr;
RGB g_cube_fb[8][8][8];         // Output frame buffer - final frame sent to shift registers
RGB g_rgb_buff[8][8][8];        // RGB frame buffer - current frame being processed
RGB g_color_buff[8][8][8];      // Color framebuffer - holds color info for entire cube. This allows us to precalculate all colors
                                // in the cube space and we can 'pull' them out as the voxels are being drawn.
RGB g_rgb_color;                // Dynamic color which will be assigned to active voxel
RGB_OPTS g_rgb_opts;

#ifdef	__cplusplus
}
#endif

#endif


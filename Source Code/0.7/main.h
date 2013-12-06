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

volatile uint16 g_ms_ctr;       // millisecond counter
volatile uint16 g_fade_ctr;
RGB g_cube_fb[8][8][8];         // 3D cube framebuffer
RGB g_rgb_buff[8][8][8];        // RGB frambuffer
RGB g_rgb_color;                // Dynamic color which will be assigned to active voxel
RGB_OPTS g_rgb_opts;

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */


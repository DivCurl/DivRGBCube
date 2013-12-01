#ifndef MAIN_H
#define	MAIN_H

#include "defs.h"
#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

RGB cube_fb[8][8][8];  // 3D cube framebuffer
RGB g_rgb_color;      // Dynamic color which will be assigned to active voxel
RGB rgb_buff[8][8][8];  // 3D cube framebuffer

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */


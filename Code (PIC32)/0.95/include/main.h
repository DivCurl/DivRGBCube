#ifndef MAIN_H
#define	MAIN_H

#include <p32xxxx.h>    // PIC32 specifics header file.
#include <plib.h>       // PIC32 Peripheral Library.
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "defs.h"
#include "types.h"
#include "serial.h"
#include "rgb.h"
#include "sa.h"

#ifdef	__cplusplus
extern "C" {
#endif

// GLOBALS
extern counter_t sa_peak_ctr;
// Cube buffers. 
// RGB type = 6 bytes (see types.h), 6*8*8*8*3 = 9216 bytes ~ 9KB total per frame buffer
extern rgb_t g_frameBuffer[8][8][8];        // Output frame buffer - final frame sent to shift registers
extern rgb_t g_colorBuffer[8][8][8];        // RGB frame buffer - current frame being processed
extern rgb_t *g_colors;                  // RGB color array
extern rgb_t g_rgb;                     // global color set by external device
extern volatile struct animNode_t *head;
extern volatile struct animNode_t *curr;
extern volatile counter_t frmCtr;
extern volatile counter_t rgbCtr;
extern anim_t anims[64];
extern anim_t *ptrAnim;
extern uint16 anIndex;

extern inline uint64_t L_ReadByte(void);
extern void I_InitConfig();
extern void I_InitLCD();
extern void I_InitAnim();

#ifdef	__cplusplus
}
#endif

#endif


#ifndef TYPES_H
#define	TYPES_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef uint8_t         uint8;
typedef uint16_t        uint16;
typedef uint32_t        uint32;
typedef uint64_t        uint64;
typedef int8_t          int8;
typedef int16_t         int16;
typedef int32_t         int32;
typedef int64_t         int64;
typedef unsigned char   uchar;
typedef uint8_t         _bool;
#define bool            _bool

// Preconfigured animations
// NOTE: animation_t should be a structure holding the name and information
// (ie. settings, framerate, etc) about the current animation.
// I want to build a linked list of animation_t's so I can change these
// parameters easily at runtime


enum {
    SHIFT_UP,
    SHIFT_DOWN,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    SHIFT_IN,
    SHIFT_OUT
};

enum {
    AN_SINE_2D,
    AN_SINE_3D,
    AN_RAIN,
    AN_CUBE_FILL_DAZ,
    AN_TEXT_FLY,
    AN_BITMAP_PATH,
    AN_CIRCLE_ROTATE,
    AN_VU_METER,
    AN_SA,
    AN_CUBE_SOLID,
    AN_FIREWORKS,
    AN_CROSS_HATCH,
    AN_FACE_PALETTE_SWAP,
    AN_HELIX,
    AN_DBL_HELIX,
    AN_DNA,
    AN_SPLASH,
    AN_TEST,
    AN_BREAK
};

enum {
    RGB_MODE_DEF,
    RGB_MODE_SOLID,
    RGB_MODE_FADE
};

typedef struct {
    int16 r;
    int16 g;
    int16 b;
} rgb_t;

typedef struct {
    bool en;        // counter enabled
    bool dn;        // counter done
    long pre;       // counter preset ticks
    long acc;       // counter accumulated ticks
   // void (*ctrGetAcc) (int * a);
} counter_t;

// Option flag defs
#define RGB_SET_NEGATIVE_COLOR  0x01
#define RGB_FADE_MODE_CHANGED   0x02
#define RGB_FADE_RATE_CHANGED   0x04

typedef struct {
    uint32          FLAGS;      // general option flags
    uint16          fadeMode;   // RGB fade mode e.g. solid
    float           angle;      // RGB color wheel angle
    uint32          rate;       // rate of change for dynamic fade modes
    rgb_t           rgbSolid;
    rgb_t           rgbMask;    // color to be applied to voxels that are off if such mode is used
 //   counter_t       delayCtr;
} rgbopts_t;

#define ANIM_USE_GLOBAL         0x01
#define ANIM_FIRST_RUN          0x02
#define ANIM_DISABLED           0x04
#define ANIM_RUN_FOREVER        0x08
#define ANIM_RATE_CHANGED       0x10
#define ANIM_PAUSE              0x20

// Allows common options to be set, either from external interface (touchscreen)
// or internally
typedef struct {
    int             id;
    uint32          FLAGS;
    uint32          its;
    uint32          it;
    uint32          rate;
//    counter_t       delayCtr;
    rgbopts_t       rgbOpts;
} anim_t;

struct gblOpts_t {
    uint32      rRate;
    uint32      fRate;
    uint32      AFLAGS;
    uint32      RFLAGS;
    uint32      rgbMode;
    rgb_t       color;

};

struct gblOpts_t gblOpts;

// for testing idea of using linked list to store active animations
struct animNode_t {
    anim_t anim;
    struct animNode_t *next;
};

#ifdef	__cplusplus
}
#endif

#endif


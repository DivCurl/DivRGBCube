#ifndef TYPES_H
#define	TYPES_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

typedef uint8_t     uint8;
typedef uint16_t    uint16;
typedef uint32_t    uint32;
typedef int8_t      int8;
typedef int16_t     int16;
typedef int32_t     int32;

typedef enum {
    SINE_2D,
    SINE_3D,    // = 0
    TEST = 99   
} ANIMATION;

// Common RGB color identifiers
typedef enum {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    VIOLET,
    MAGENTA
} SOLID_RGB;

// DELETE IF NOT NEEDED
typedef struct {
    uint8 r;
    uint8 g;
    uint8 b;
} RGB;

#ifdef	__cplusplus
}
#endif

#endif


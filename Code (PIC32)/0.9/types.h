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
    typedef uint8_t     Bool;
    #define bool        _Bool

    typedef enum {
        SINE_2D,
        SINE_3D,
        RAIN,
        CUBE_FILL_DAZ,
        TEXT_FLY,
        TEST,
        BREAK
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

    typedef struct {
        int16 r;
        int16 g;
        int16 b;
    } RGB;

    typedef enum {
        FADE_CW_CONT,        // continuous fade clockwise around entire RGB space
        FADE_CCW_CONT,       // continuous fade counter-clockwise around entire RGB space
        FADE_CW_INPUT,       // continuous fade between specified input colors, clockwise
        FADE_CCW_INPUT,      // continuous fade between specified input colors
        FADE_PREBAKED,
        FADE_WIPE,           // 'wipe' effect to transision entire cube from one color to another
        SOLID_COLOR,         // use a solid color for entire cube
    } COLOR_FADE_MODE;

    typedef struct {
        uint8 x;        // current x-coord of voxel
        uint8 y;        // current y-coord of voxel
        uint8 z;        // current z-coord of voxel
        uint16 delay;   // frame draw delay...would be nice to specify as framerate
        uint16 fade_delay;  // RGB fade delay
        uint16 angle_roc;   // rate of change for angle increase
        uint16 its;     // number of iterations
        SOLID_RGB opt1;     // opt1 thru opt4 are optional parameters used by drawing functions
        int16 opt2;
        int16 opt3;
        int16 opt4;
        COLOR_FADE_MODE fade_mode;  // RGB fade mode
    } RGB_OPTS;


    #ifdef	__cplusplus
    }
    #endif

#endif


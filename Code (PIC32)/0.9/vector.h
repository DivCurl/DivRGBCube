#ifndef VECTOR_H
    #define VECTOR_H

    #ifdef	__cplusplus
    extern "C" {
    #endif

    #include "main.h"
    #include "math.h"

    typedef struct {
        // column-major 4x4 matrix
        int16 vec[4];
    } VECTOR4;

    typedef struct {
        // column-major 3x3 matrix
        int16 vec[3];
    } VECTOR3;

    typedef struct {
        // column-major 4x4 matrix
        int16 vec[2];
    } VECTOR2;

    #ifdef	__cplusplus
    }
    #endif

#endif



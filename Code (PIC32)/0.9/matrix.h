#ifndef MATRIX_H
    #define MATRIX_H

    #ifdef	__cplusplus
    extern "C" {
    #endif

    #include "main.h"
    #include "math.h"
    #include "vector.h"

    typedef struct {
        // column-major 4x4 matrix
        int16 mx[16];
    } MATRIX4;

    typedef struct {
        // column-major 3x3 matrix
        int16 mx[9];
    } MATRIX3;

    typedef struct {
        // column-major 2x2 matrix
        int16 mx[4];
    } MATRIX2;

    MATRIX3 MXTranslate2D(int16 dx, int16 dy);
    MATRIX4 MXTranslate3D(int16 dx, int16 dy, int16 dz);
    MATRIX4 MXRotate3D(float alpha, float beta, float gamma);
    MATRIX3 MXRotate2D(float theta);
    VECTOR3 MXMulVec2D(MATRIX3 mx, VECTOR3 vec);
    VECTOR4 MXMulVec3D(MATRIX4 mx, VECTOR4 vec);


    #ifdef	__cplusplus
    }
    #endif

#endif


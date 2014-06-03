#ifndef MATRIX_H
#define MATRIX_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "types.h"
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

MATRIX3 M_Translate2D( int16 dx, int16 dy );
MATRIX4 M_Translate3D( int16 dx, int16 dy, int16 dz );
MATRIX4 M_Rotate3D( float alpha, float beta, float gamma );
MATRIX3 M_Rotate2D( float theta );
VECTOR3 M_MulVec2D( MATRIX3 mx, VECTOR3 vec );
VECTOR4 M_MulVec3D( MATRIX4 mx, VECTOR4 vec );

#ifdef	__cplusplus
}
#endif

#endif


#include "../include/matrix.h"
#include "../include/vector.h"

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Mul4(MATRIX4 mx1, MATRIX4 mx2) {
    MATRIX4 tmpMx;
    uint8 row, col;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            // since matrix is stored as 1D array, offset column by 4 on
            // each iteration count to ensure columns line up
            tmpMx.mx[col * 4 + row] =
                mx1.mx[0 + row] * mx2.mx[0 + col * 4] +
                mx1.mx[4 + row] * mx2.mx[1 + col * 4] +
                mx1.mx[8 + row] * mx2.mx[2 + col * 4] +
                mx1.mx[12 + row] * mx2.mx[3 + col * 4];
        }
    }
    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Add4(MATRIX4 mx1, MATRIX4 mx2) {
    MATRIX4 tmpMx;
    uint8 i;
    for (i = 0; i < 16; i++)
        tmpMx.mx[i] = mx1.mx[i] + mx2.mx[i];

    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_MulScalar4(MATRIX4 inMx, float s) {
    uint8 i;
    for (i = 0; i < 16; i++)
        inMx.mx[i] *= s;

    return inMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_DivScalar4(MATRIX4 inMx, float s) {
    uint8 i;
    for (i = 0; i < 16; i++)
        inMx.mx[i] /= s;

    return inMx;
}

/*
 * | 1 0 0 0 |
 * | 0 1 0 0 |
 * | 0 0 1 0 |
 * | 0 0 0 1 |
 *
 */

/*
===================
 Name

 Desc
===================
*/
void M_Identity4(MATRIX4 *mxptr) {
    mxptr->mx[0] = 1;   mxptr->mx[4] = 0;   mxptr->mx[8] = 0;   mxptr->mx[12] = 0;
    mxptr->mx[1] = 0;   mxptr->mx[5] = 1;   mxptr->mx[9] = 0;   mxptr->mx[13] = 0;
    mxptr->mx[2] = 0;   mxptr->mx[6] = 0;   mxptr->mx[10] = 1;  mxptr->mx[14] = 0;
    mxptr->mx[3] = 0;   mxptr->mx[7] = 0;   mxptr->mx[11] = 0;  mxptr->mx[15] = 1;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Transpose4() {

}

/*
===================
 Name

 Desc
===================
*/
float M_Dot4() {

}

/*
===================
 Name

 Desc
===================
*/
float M_Det4() {

}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_Mul3(MATRIX3 mx1, MATRIX3 mx2) {
    MATRIX3 tmpMx;
    uint8 row, col;

    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            // since matrix is stored as 1D array, offset column by 3 on
            // each iteration count to ensure everything lines up
            tmpMx.mx[col * 3 + row] =
                mx1.mx[0 + row] * mx2.mx[0 + col * 3] +
                mx1.mx[3 + row] * mx2.mx[1 + col * 3] +
                mx1.mx[6 + row] * mx2.mx[2 + col * 3];
        }
    }
    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_Add3(MATRIX3 mx1, MATRIX3 mx2) {
    MATRIX3 tmpMx;
    uint8 i;
    for (i = 0; i < 9; i++)
        tmpMx.mx[i] = mx1.mx[i] + mx2.mx[i];

    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_MulScalar3(MATRIX3 inMx, float s) {
    uint8 i;
    for (i = 0; i < 9; i++)
        inMx.mx[i] *= s;

    return inMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_DivScalar3(MATRIX3 inMx, float s) {
    uint8 i;
    for (i = 0; i < 9; i++)
        inMx.mx[i] /= s;

    return inMx;
}

/*
===================
 Name

 Desc
===================
*/
void M_Identity3(MATRIX3 *mxptr) {
    mxptr->mx[0] = 1;   mxptr->mx[3] = 0;   mxptr->mx[6] = 0;
    mxptr->mx[1] = 0;   mxptr->mx[4] = 1;   mxptr->mx[7] = 0;
    mxptr->mx[2] = 0;   mxptr->mx[5] = 0;   mxptr->mx[8] = 1;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_Transpose3() {

}

/*
===================
 Name

 Desc
===================
*/
float M_Dot3() {

}

/*
===================
 Name

 Desc
===================
*/
float M_Det3() {

}


/* Visualization of 1-D array layout for reference
     | 0  2 |
     | 1  3 |
 */

/*
===================
 Name

 Desc
===================
*/
MATRIX2 M_Mul2(MATRIX2 mx1, MATRIX2 mx2) {
    MATRIX2 tmpMx;
    uint8 row, col;

    for (row = 0; row < 2; row++) {
        for (col = 0; col < 2; col++) {
            tmpMx.mx[col * 2 + row] =
                mx1.mx[0 + row] * mx2.mx[0 + col * 2] +
                mx1.mx[2 + row] * mx2.mx[1 + col * 2];
        }
    }
    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX2 M_Add2(MATRIX2 mx1, MATRIX2 mx2) {
    MATRIX2 tmpMx;
    uint8 i;
    for (i = 0; i < 4; i++)
        tmpMx.mx[i] = mx1.mx[i] + mx2.mx[i];

    return tmpMx;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX2 M_MulScalar2(MATRIX2 inMx, float s) {
    uint8 i;
    for (i = 0; i < 4; i++)
        inMx.mx[i] *= s;

    return inMx;
}

/*
===================
 Name

 Desc
===================
*/
 MATRIX3 M_Scale2D(int16 sx, int16 sy) {
    MATRIX3 mxTmp;
    // Make an identity matrix first
    M_Identity3(&mxTmp);
    mxTmp.mx[0] = sx;
    mxTmp.mx[4] = sy;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Scale3D() {

}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_Translate2D(int16 dx, int16 dy) {
    MATRIX3 mxTmp;
    // Make an identity matrix first
    M_Identity3(&mxTmp);
    mxTmp.mx[6] = dx;
    mxTmp.mx[7] = dy;

    return mxTmp;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Translate3D(int16 dx, int16 dy, int16 dz) {
    MATRIX4 mxTmp;
    // Make an identity matrix first
    M_Identity4(&mxTmp);
    mxTmp.mx[12] = dx;
    mxTmp.mx[13] = dy;
    mxTmp.mx[14] = dz;

    return mxTmp;
}

/*
===================
 Name

 Desc
===================
*/
MATRIX3 M_Rotate2D(float theta) {
    MATRIX3 mxTmp;
    // Make an identity matrix first
    M_Identity3(&mxTmp);

    mxTmp.mx[0] = D_Round(cos(theta));
    mxTmp.mx[1] = D_Round(sin(theta));
    mxTmp.mx[3] = D_Round(-sin(theta));
    mxTmp.mx[4] = D_Round(cos(theta));

    return mxTmp;
}

// Working, more testing reqd.
// Noticed limitations due to cube 'display' resolution being fairly low,
// complex objects don't look too hot when rotated through fractional angles
/*
===================
 Name

 Desc
===================
*/
MATRIX4 M_Rotate3D(float alpha, float beta, float gamma) {
    MATRIX4 mxTmp;
    M_Identity4(&mxTmp);

    mxTmp.mx[0] = D_Round(cos(beta)*cos(gamma));
    mxTmp.mx[1] = D_Round(cos(beta)*sin(gamma));
    mxTmp.mx[2] = D_Round(-sin(beta));

    mxTmp.mx[4] = D_Round((cos(gamma)*sin(alpha)*sin(beta)) - (cos(alpha)*sin(gamma)));
    mxTmp.mx[5] = D_Round((cos(alpha)*cos(gamma)) + (sin(alpha)*sin(beta)*sin(gamma)));
    mxTmp.mx[6] = D_Round((cos(beta)*sin(alpha)));

    mxTmp.mx[8] = D_Round((cos(alpha)*cos(gamma)*sin(beta)) + (sin(alpha)*sin(gamma)));
    mxTmp.mx[9] = D_Round((-cos(gamma)*sin(alpha)) + cos(alpha)*sin(beta)*sin(gamma));
    mxTmp.mx[10] = D_Round(cos(alpha)*cos(beta));

    return mxTmp;
}

/*  | 0 3 6 |   | 0 |
 *  | 1 4 7 | * | 1 |
 *  | 2 5 8 |   | 2 |
 */
// Multiply 3X3 matrix by 3x1 VECTOR. Returns 3x1 Vector
// Working
/*
===================
 Name

 Desc
===================
*/
VECTOR3 M_MulVec2D(MATRIX3 mx, VECTOR3 vec) {
    VECTOR3 ptVec;
    int row;
    for (row = 0; row < 3; row++) {
        ptVec.vec[row] =
            mx.mx[0 + row] * vec.vec[0] +
            mx.mx[3 + row] * vec.vec[1] +
            mx.mx[6 + row] * vec.vec[2];
    }
    return ptVec;
}

// Multiply 4X4 matrix by 4x1 VECTOR. Returns 4x1 vector
// Working
/*
===================
 Name

 Desc
===================
*/
VECTOR4 M_MulVec3D(MATRIX4 mx, VECTOR4 vec) {
    VECTOR4 ptVec;
    int row;
    for (row = 0; row < 4; row++) {
        ptVec.vec[row] =
            mx.mx[0 + row] * vec.vec[0] +
            mx.mx[4 + row] * vec.vec[1] +
            mx.mx[8 + row] * vec.vec[2] +
            mx.mx[12 + row] * vec.vec[3];
    }
    return ptVec;
}
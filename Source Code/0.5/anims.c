#include "anims.h"
#include "driver.h"

// Animation Sequence Handler
void Animate(uint8 index) {
    // Assign the currnet animation based on the counter
    ANIMATION anim = index;

    // WOULD LIKE A MORE FLEXIBLE WAY TO SET DELAYS?
    uint16 speed, repeat_cnt;

    switch (anim) {
        case (SINE_2D):
            SineWave2D();
            break;
        case (SINE_3D) :
            SineWave3D();
            break;
        case (CUBE_TEST):
            CubeTest();
            break;
        default:
            break;
    }
}

void SineWave2D() {
    // Working.
    // TODO: set voxel colors dynamically
    static float x, z, phi;
    int count;

    ClrCube();
    for ( x= 0; x <= X_MAX; x++ ) {

        z = sin(x + phi); //
        z = Round(Remap(z,-1, 1, 0, Z_MAX));
        // Working with solid colors for now. Revise color fading code.
        VoxelSetSolidRGB(x, 2, z, RED);
        VoxelSetSolidRGB(x, 3, z, GREEN);
        VoxelSetSolidRGB(x, 4, z, BLUE);
        VoxelSetSolidRGB(x, 5, z, RED);
	
    }
    // This is just a quick test hack to slow down the frame rate. Need to use a timer for proper delay operation.
    for (count = 0; count < 20000; count++) {}

    phi += PI/8;
    if (phi >= 2*PI)
        phi = 0;
}

void SineWave3D() {
    // Plots a surface sine wave of form z = sin(sqrt(x^2 + y^2) + phi)
    // Note: Working, but at slower framerates produces some flickering of the LEDs the closer they
    // approach the x=7 plane. Probably something to do with calculations/values being produced by the sin() function
    static int x, y, Z;
    static float phi;
    float i, j, z;
    int count;

    ClrCube();
    for (x = 0; x <= X_MAX; x++ ) {
        for (y = 0; y <= Y_MAX; y++) {
            i = Remap(x, 0, 7, -PI, PI);
            j = Remap(y, 0, 7, -PI, PI);
            z = sin((sqrt(i*i + j*j)) + phi );
            Z = Round(Remap(z,-1,1,0,Z_MAX));
            if ((Z%2)== 0)
                VoxelSetSolidRGB(x,y,Z, CYAN);
            else
                VoxelSetSolidRGB(x,y,Z, VIOLET);
        }
    }
    // This is just a quick test hack to slow down the frame rate. Need to use a timer for proper delay operation.
  // for (count = 0; count < 1000; count++) {}
    phi += PI/7;
    if (phi >= 2*PI)
        phi = 0;

 }

// Test Function - lights all LEDs in sequence from (0,0,0) to (7,7,7) to confirm basic operation
void CubeTest(void) {
// Simple routine for testing only. Lights up all LEDs with a different color on each x-y plane.
// Useful for ensuring BCM and basic software/hardware functions are working properly

    uint8 i, j;
    //VoxelSetSolidRGB(0,0,1, RED);

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 0, RED);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 1, GREEN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 2, BLUE);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 3, ORANGE);
        }
    }

     for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 4, VIOLET);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 5, YELLOW);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 6, CYAN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            VoxelSetSolidRGB(i, j, 7, MAGENTA);
        }
    }

}

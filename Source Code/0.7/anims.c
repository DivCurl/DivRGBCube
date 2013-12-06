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
            Sine_Wave_2D();
            break;
        case (SINE_3D) :
            Sine_Wave_3D();
            break;
        case (RAIN) :
            Rain();
            break;
        case (CUBE_TEST):
            Cube_Test();
            break;
        default:
            break;
    }
}

// Working, needs tuning to timing
void Rain() {
    uint8 i, r_x, r_y, rnd_num;
    
    g_rgb_opts.its = 1000;
    g_rgb_opts.fade_mode = FADE_CW_CONT;
    g_rgb_opts.fade_delay = 50;
    g_rgb_opts.angle_roc = 10;

    
    while (!(Delay_mS(150))) {}
    rnd_num = rand()%5;
    for (i=0; i < rnd_num;i++) {
        r_x = rand()%8;
        r_y = rand()%8;
          Voxel_Set_RGB(r_x, r_y, 7, Get_Color_RGB());
      //    Voxel_Set_Solid_RGB(r_x, r_y, 7, RED);
    }
    // Note: framebuffer update is handled in Shift() function
    Shift('Z', DOWN);
}

// Working
void Sine_Wave_2D() {
    // Working.
    // TODO: set voxel colors dynamically
    static float x, z, phi;
    int count;
    while (!(Delay_mS(1500))) {}
    Clear_Frame();
    for (x = 0; x <= X_MAX; x++ ) {

        z = sin(x + phi); //
        z = Round(Remap(z,-1, 1, 0, Z_MAX));
        // Working with solid colors for now. Revise color fading code.
        Voxel_Set_Solid_RGB(x, 2, z, RED);
        Voxel_Set_Solid_RGB(x, 3, z, GREEN);
        Voxel_Set_Solid_RGB(x, 4, z, BLUE);
        Voxel_Set_Solid_RGB(x, 5, z, RED);
    }

    phi += PI/8;
    if (phi >= 2*PI)
        phi -= 2*PI;

    FB_Update();
}

// Working
void Sine_Wave_3D() {
    // Plots a surface sine wave of form z = sin(sqrt(x^2 + y^2) + phi)
    // Note: Working, but at slower framerates produces some flickering of the LEDs the closer they
    // approach the x=7 plane. Probably something to do with calculations/values being produced by the sin() function
    //    uint8 frame_dwell;
    static uint8 x, y, Z;
    static float phi;
    float i, j, z;

    g_rgb_opts.fade_mode = FADE_CW_CONT;
    g_rgb_opts.fade_delay = 100;
    while (!(Delay_mS(30))) {}
    Clear_Frame();   // Clear the current frame before updating it
    for (x = 0; x <= X_MAX; x++ ) {
        for (y = 0; y <= Y_MAX; y++) {
            i = Remap(x, 0, 7, -PI, PI);
            j = Remap(y, 0, 7, -PI, PI);
            z = sin((sqrt(i*i + j*j)) + phi );
            Z = Round(Remap(z,-1,1,0,Z_MAX));
            Voxel_Set_RGB(x,y,Z, Get_Color_RGB());
        }
    }    
    
    // note: The smaller the phase increment, the slower the draw speed
    phi += PI/7;
    if (phi >= 2*PI)
        phi -= 2*PI;

    FB_Update();
 }

// Test Function - lights all LEDs in sequence from (0,0,0) to (7,7,7) to confirm basic operation
void Cube_Test(void) {
// Simple routine for testing only. Lights up all LEDs with a different color on each x-y plane.
// Useful for ensuring BCM and basic software/hardware functions are working properly

    uint8 i, j;
    //VoxelSetSolidRGB(0,0,1, RED);

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 0, RED);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 1, GREEN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 2, BLUE);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 3, ORANGE);
        }
    }

     for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 4, VIOLET);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 5, YELLOW);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 6, CYAN);
        }
    }

    for (i = 0; i <= 7; i++) {
        for (j = 0; j <= 7; j++) {
            Voxel_Set_Solid_RGB(i, j, 7, MAGENTA);
        }
    }

}

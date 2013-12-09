#include "anims.h"
#include "driver.h"

// Animation Sequence Handler
void Animate(uint8 index, uint16 its) {
    int32 count = 0;       // current iteration count
    ANIMATION anim = index;

    switch (anim) {
        case (SINE_2D):
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                Refresh_Color_Buffer(); 
                Sine_Wave_2D();
                count++;
            }
            break;
        case (SINE_3D) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                Refresh_Color_Buffer();
                Sine_Wave_3D();
                count++;
            }
            break;
        case (RAIN) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                Refresh_Color_Buffer();
                Rain();
                count++;
            }
            break;
        case (CUBE_FILL_DAZ) :
            g_rgb_opts.fade_mode = FADE_CW_CONT;
            while (count < its) {
                Refresh_Color_Buffer();
                Cube_Fill_Dazzle();
                count++;
            }
            break;
        case (CUBE_TEST):
            Cube_Test();
            break;
        default:
            break;
    }
}

// Working
void Rain() {
    uint8 i, r_i, r_ii, rnd_num;
    
    g_rgb_opts.its = 250;
    g_rgb_opts.fade_mode = FADE_CW_CONT;
    g_rgb_opts.fade_delay = 10;
    g_rgb_opts.angle_roc = 2;
    
    while (!(Delay_mS(100))) {}
    rnd_num = rand()%5;
    for (i=0; i < rnd_num; i++) {
        r_i = rand()%8;
        r_ii = rand()%8;
          Voxel_Set_RGB(r_i, r_ii, 7, Get_Color_RGB(r_i, r_ii, 7));
    }
    // Note: framebuffer update is handled in Shift() function
    Shift('Z', DOWN);
}

// Working
void Sine_Wave_2D() {
    static float phi;
    static uint8 x, y, Z;
    float z;
    g_rgb_opts.fade_delay = 500;
    g_rgb_opts.angle_roc = 2;

    while (!(Delay_mS(100))) {}
    Clear_Frame();
    for (x = 0; x <= X_MAX; x++ ) {

        z = sin(x + phi); 
        Z = Round(Remap(z, -1, 1, 0, Z_MAX));
        Voxel_Set_Solid_RGB(x, 1, Z, BLUE);
        Voxel_Set_RGB(x, 2, Z, Get_Color_RGB(x, x, Z));
        Voxel_Set_RGB(x, 3, Z, Get_Color_RGB(x, x, Z));
        Voxel_Set_RGB(x, 4, Z, Get_Color_RGB(x, x, Z));
        Voxel_Set_RGB(x, 5, Z, Get_Color_RGB(x, x, Z));
        Voxel_Set_Solid_RGB(x, 6, Z, BLUE);
    }

    phi += PI/8;
    if (phi >= 2*PI)
        phi -= 2*PI;

    FB_Update();
}

// Working
void Sine_Wave_3D() {
    static uint8 x, y, Z;
    static float phi;
    float i, j, z;

    g_rgb_opts.fade_delay = 10;
    g_rgb_opts.angle_roc = 2;

    while (!(Delay_mS(40))) {}
    Clear_Frame();   // Clear the current frame before updating it
    for (x = 0; x <= X_MAX; x++ ) {
        for (y = 0; y <= Y_MAX; y++) {
            i = Remap(x, 0, 7, -PI, PI);
            j = Remap(y, 0, 7, -PI, PI);
            z = sin((sqrt(i*i + j*j)) + phi );
            Z = Round(Remap(z,-1,1,0,Z_MAX));
            Voxel_Set_RGB(x,y,Z, Get_Color_RGB(x, y, Z));
         }
    }
    FB_Update();
    
    // note: The smaller the phase increment, the slower the draw speed
    phi += PI/7;
    if (phi >= 2*PI)
        phi -= 2*PI;  
 }

/* Fill cube one voxel at time
 * Start by sending a voxel flying down x=0, z=0 axis and stop at (0,7,0).
 * Continue filling this axis until all y-voxels on x=0 are filled. Then move to x=1, z=0 axis.
 * Continue filling until all voxels on this plane are filled, then move up to next plane and
 * repeat until entire cube is filled
 */
Cube_Fill_Dazzle() {
    int16 i, ii, j;

    g_rgb_opts.fade_delay = 250;
    g_rgb_opts.angle_roc = 2;

    Clear_Frame();

    for (j = 0; j <= 7; j++) {
        for (ii = 0; ii <= 7; ii++) {         // Y-offset
            for (i = 7; i >= 0; i--) {        // X-offset
                // params: plane, start, end, ortho-offset, perp-offset, delay
                Voxel_Fly("XY", 0, i, ii, j, 25);
                Refresh_Color_Buffer();
            }
        }
    }
}
/* Simple routine for testing only. Lights up all LEDs with a different color on each x-y plane.
 * Useful for ensuring BCM and basic software/hardware functions are working properly
 */
void Cube_Test(void) {
    uint8 i, j;

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

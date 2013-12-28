#include "main.h"

/*** Pragmas for setting up PIC32 osillator and chip ***/
#pragma config POSCMOD      = HS        // Enable high speed osillator module
#pragma config FNOSC        = PRIPLL    // use primary PLL
#pragma config FPLLIDIV     = DIV_2     // Divide 8MHz crystal clock by 2 for 4MHz to PLL input
#pragma config FPLLMUL      = MUL_20    // Multiply PLL by 15 (=80MHz)
#pragma config FPLLODIV     = DIV_1     // PLL output divide by 1 (=80MHZ Fcy)
#pragma config FPBDIV       = DIV_8     // Divide core clock by 2 for peripheral bus (=20MHz Fpb)
#pragma config FWDTEN       = OFF       // Disable Watchdog timer
#pragma config CP           = OFF       // Disable code protect
#pragma config BWP          = OFF       // Disable boot write protect
#pragma config PWP          = OFF       // Program program write Protect

// Globals
volatile uint8 rgb_buff_full = 0;
volatile uint8 cube_layer = 0;
volatile uint16 bcm_bit_tick = 0;
volatile uint16 next_bcm_bit_tick = 1;
volatile uint8 bcm_bit_pos = 0;
int test_ctr = 0;

void __ISR (_TIMER_2_VECTOR, ipl2) TMR2IntHandler(void) {
    int8 x, y, z;
    z = cube_layer;
    
    // Working
    for (y = Y_MAX; y >= 0; y--) {
        for (x = X_MAX; x >= 0; x--) {
            // Odd-numbered Y-offsets are wired backwards (due to my own impatience).
            // Need to reverse the shift order on the X-axis in this case.
            // Remove the (7-x) X-offset hack if/when the wiring is corrected.
            if ((y % 2) != 0) {
                if ((g_cube_fb[(7-x)][y][z].r >> bcm_bit_pos))
                    LATBSET = SR_SDI_R;

                if ((g_cube_fb[(7-x)][y][z].g >> bcm_bit_pos))
                    LATBSET = SR_SDI_G;

                if ((g_cube_fb[(7-x)][y][z].b >> bcm_bit_pos))
                    LATBSET = SR_SDI_B;
            }
            // If we're on an even-numbered Y-offset, shift the data normally.
            else {
                if ((g_cube_fb[x][y][z].r >> bcm_bit_pos))
                    LATBSET = SR_SDI_R;

                if ((g_cube_fb[x][y][z].g >> bcm_bit_pos))
                    LATBSET = SR_SDI_G;

                if ((g_cube_fb[x][y][z].b >> bcm_bit_pos))
                    LATBSET = SR_SDI_B;
            }

            // Send current voxel color data to SRs
            LATBSET = SR_CLK;
            LATBCLR = SR_CLK | SR_SDI_R | SR_SDI_G | SR_SDI_B;
            } // X axis
    } // Y axis

      // Turn on the current layer und blinken ze lights
      /* Note: The short delay in between each layer switch is to ensure
       * that the PIC32 has fully transitioned the PORT outputs. Without the delay,
       * I've noticed the layer transistor previously ON appears to still conduct slightly
       * resulting in unwanted albeit very dim lighting of the previous layer.
       * Probably due to the transistors I used not being able to tolerate the
       * extremely fast port switching speed.
       */
    uint16 i;
    switch (cube_layer) {
        case (0) :
            LATESET = LAYER_Q7;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q0;
             break;
        case (1) :
            LATESET = LAYER_Q0;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q1;
             break;
        case (2) :
            LATESET = LAYER_Q1;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q2;
             break;
        case (3) :
            LATESET = LAYER_Q2;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q3;
             break;
        case (4) :
            LATESET = LAYER_Q3;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q4;
            break;
        case (5) :
            LATESET = LAYER_Q4;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q5;
            break;
        case (6) :
            LATESET = LAYER_Q5;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q6;
            break;
        case (7) :
            LATESET = LAYER_Q6;
            for (i = 0; i < Q_DELAY; i++) {}
            LATECLR = LAYER_Q7;
            break;
        default:
            break;
    } // layer switch

   // Latch SR outputs
    LATBSET = SR_LE;
    LATBCLR = SR_LE;
        
    // Layer refresh complete. Increment layer num for next interrupt and return
    if (cube_layer < Z_MAX)
        cube_layer++;
    else
        cube_layer = 0;

     /* Handle BCM stuff before returning from interrupt */
     bcm_bit_tick++;

    // in BCM operation, next_bit_tick corresponds to the (n)th cycle of the refresh period. The value is updated once
    // TMR0 has accumulated the corresponding number of ticks (2^(nth) counts) for this cycle.
    if (bcm_bit_tick == next_bcm_bit_tick) {
            next_bcm_bit_tick <<=  1;
            bcm_bit_pos++;
    }

    // Reset all once refresh period is complete (8 full cycles processed)
    if (bcm_bit_pos == BCM_BIT_MAX) {
            bcm_bit_tick = 0;
            bcm_bit_pos = 0;
            next_bcm_bit_tick = 1;
    }

    mT2ClearIntFlag();
}

// For use with millisecond tick counter
void __ISR (_TIMER_4_VECTOR, ipl3) TMR4IntHandler(void) {
    g_ms_ctr = g_ms_ctr++;
    mT4ClearIntFlag();
}

int main() {
    uint8 anim_ctr;
    // Performance optimization macro
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    TRISB = 0x0000;         // Set all port pins to OUTPUT.
    TRISE = 0x0000;         // Set all port pins to OUTPUT.
    DDPCONbits.JTAGEN = 0;  // Disable JTAG port
    // Timer 2 used for main display refresh interrupt
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_1, T2_INT_PER); 
    ConfigIntTimer2( T2_INT_ON | T2_INT_PRIOR_2);
    // Timer 4 used for general timing
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, 50);
    ConfigIntTimer4( T4_INT_ON | T4_INT_PRIOR_3);
    INTEnableSystemMultiVectoredInt();
    // Start with all layer transistors ON (disable all layers)
    LATESET = LAYER_Q0 | LAYER_Q1 | LAYER_Q2 | LAYER_Q3 | LAYER_Q4 |
            LAYER_Q6 | LAYER_Q7;
    // Clear all signals on PORTB on startup
    LATBCLR = SR_OE | SR_LE | SR_CLK | SR_SDI_R | SR_SDI_G | SR_SDI_B;

    while (1) {
    // ** FUTURE - randomize animations at runtime ** //
        
        Animate(SINE_2D, 500);
        Animate(SINE_3D, 500);
        Animate(RAIN, 500);
        Animate(CUBE_FILL_DAZ, 1);
        Animate(TEXT_FLY, 1);


//        Animate(TEST);        // for testing only
       
     } // main loop
    
    return (EXIT_SUCCESS);
}

#ifndef DEFS_H
#define	DEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SYS_FREQ        (80000000L)
#define PB_DIV          2
#define PI              3.14159265358979323846
#define TRUE            1
#define FALSE           0
#define X_SIZE          8
#define Y_SIZE          8
#define Z_SIZE          8
#define X_MAX           7 // 0 inclusive
#define Y_MAX           7 // 0 inclusive
#define Z_MAX           7 // 0 inclusive
#define LAYER_MAX       7 // 0 inclusive
#define ANIM_MAX        1 // 0 inclusive
#define BCM_BIT_MAX     7 // 0 inclusive
#define COLOR_MAX       512
#define Q_DELAY         10

// Pin definintions for shift register signals.
#define SR_CLK          0x01        // Wired to PORTB output 0
#define SR_LE           0x02        // Wired to PORTB output 1
#define SR_OE           0x04        // Wired to PORTB output 2
#define SR_SDI_R        0x08        // Wired to PORTB output 3
#define SR_SDI_G        0x10        // Wired to PORTB output 4
#define SR_SDI_B        0x20        // Wired to PORTB output 5

// Pin defintions for MUX layer control transistors
#define LAYER_Q0        0x01        // Wired to PORTE output 0
#define LAYER_Q1        0x02        // Wired to PORTE output 1
#define LAYER_Q2        0x04        // Wired to PORTE output 2
#define LAYER_Q3        0x08        // Wired to PORTE output 3
#define LAYER_Q4        0x10        // Wired to PORTE output 4
#define LAYER_Q5        0x20        // Wired to PORTE output 5
#define LAYER_Q6        0x40        // Wired to PORTE output 6
#define LAYER_Q7        0x80        // Wired to PORTE output 7

// Other defines
#define CUBE_TEST 99

#ifdef	__cplusplus
}
#endif

#endif	/* DEFS_H */


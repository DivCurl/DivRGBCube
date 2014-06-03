#ifndef DEFS_H
#define	DEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

/* NOTE:
 * sysclock set to 40 Mhz because STP16 drivers can't tolerate
 * faster switching speeds. I've experimented with higher clocks and this seems to
 * perform best - anything faster and the drivers seem to miss clock edges.
 * Since this is only important in the main display refresh ISR, I may attempt
 * to change the clock speed on the fly when the ISR is entered and then change
 * back to 80Mhz but am unsure of what other issues this may cause and at the
 * moment nor do I have a real need to run the chip at full speed
 */
#define SYS_FREQ                    (40000000L)
#define PB_FREQ                     (40000000L)
#define UART1_BAUD                  (9600)
#define PI                          3.1415926535897932384626433832795f
#define PI_OVER_180                 PI / 180.0f
#define TWO_PI                      PI * 2.0f
#define TRUE                        1
#define FALSE                       0
#define ON                          1
#define OFF                         0
#define CUBE_X_MIN                  0
#define CUBE_X_MAX                  7 // 0 inclusive
#define CUBE_Y_MIN                  0
#define CUBE_Y_MAX                  7 // 0 inclusive
#define CUBE_Z_MIN                  0
#define CUBE_Z_MAX                  7 // 0 inclusive
#define CUBE_LAYER_MAX              7 // 0 inclusive
#define CUBE_ANIM_MAX               1 // 0 inclusive
#define CUBE_BCM_BITS_MAX           7 // 0 inclusive
#define CUBE_BCM_BITS               3 // 0 inclusive
#define CUBE_BCM_RES                ( CUBE_BCM_BITS_MAX - CUBE_BCM_BITS )
#define Q_DELAY                     25

// Pin definintions for shift register signals.
#define SR_CLK                      0x01        // Wired to PORTB output 0
#define SR_LE                       0x02        // Wired to PORTB output 1
#define SR_OE                       0x04        // Wired to PORTB output 2
#define SR_SDI_R                    0x08        // Wired to PORTB output 3
#define SR_SDI_G                    0x10        // Wired to PORTB output 4
#define SR_SDI_B                    0x20        // Wired to PORTB output 5
#define INT_T2_TEST_PIN             0x40        // Used for calibrating T2 interrupt speed - PORTB output 6
#define INT_T4_TEST_PIN             0x80        // Used for calibrating T4 interrupt speed - PORTB output 7

// Pin defintions for MUX layer control transistors
#define LAYER_Q0                    0x01        // Wired to PORTE output 0
#define LAYER_Q1                    0x02        // Wired to PORTE output 1
#define LAYER_Q2                    0x04        // Wired to PORTE output 2
#define LAYER_Q3                    0x08        // Wired to PORTE output 3
#define LAYER_Q4                    0x10        // Wired to PORTE output 4
#define LAYER_Q5                    0x20        // Wired to PORTE output 5
#define LAYER_Q6                    0x40        // Wired to PORTE output 6
#define LAYER_Q7                    0x80        // Wired to PORTE output 7
#define LAYER_ALL_Q                 ( LAYER_Q0 | LAYER_Q1 | LAYER_Q2 | LAYER_Q3 | LAYER_Q4 | LAYER_Q5 | LAYER_Q6 | LAYER_Q7 )

#define ANIM_SEQ_MAX                9
#define ANIM_USE_GLOBAL             0x01
#define ANIM_FIRST_RUN              0x02
#define ANIM_DISABLED               0x04
#define ANIM_RUN_FOREVER            0x08
#define ANIM_CHANGE_PENDING         0x10
#define ANIM_PAUSE                  0x20
#define ANIM_NO_DELAY               0x40

#define RGB_SET_NEGATIVE_COLOR      0x01
#define RGB_VOX_OFF_FADEOUT         0x02

// Messages from uLCD-70dt touchscreen via UART
#define LCD_QUERY                   0x010A0000000B
#define LCD_ACK                     0x06
#define LCD_NAK                     0x15
#define LCD_BTN_PAUSE_ON            0x071E00000118
#define LCD_BTN_PAUSE_OFF           0x071E00000019
#define LCD_BTN_RUN_FOREVER_ON      0x071E01000119
#define LCD_BTN_RUN_FOREVER_OFF     0x071E01000018
#define LCD_BTN_GLOBAL_ON           0x071E0200011A
#define LCD_BTN_GLOBAL_OFF          0x071E0200001B
#define LCD_BTN_SET_PAUSE_ON        0x011E0000011E
#define LCD_BTN_SET_PAUSE_OFF       0x011E0000001F
#define LCD_BTN_SET_STBY_ON         0x011E0100011F
#define LCD_BTN_SET_STBY_OFF        0x011E0100001E
#define LCD_BTN_SET_GLOBAL_ON       0x011E0200011C
#define LCD_BTN_SET_GLOBAL_OFF      0x011E0200001D
#define LCD_BTN_SET_TEST_ON         0x011E0300011D
#define LCD_BTN_SET_TEST_OFF        0x011E0300001C
#define LCD_BTN_AN_PREV             0x071E0500001C
#define LCD_BTN_AN_NEXT             0x071E0300001A
#define LCD_BTN_AN_SWITCH_SA        0x071E0400001D
#define LCD_BTN_AN_USE_SEQ          0xDEADBEEF

#define LCD_RED_CHG                 0x070603000002
#define LCD_ORG_CHG                 0x070604000005
#define LCD_YEL_CHG                 0x070600000001
#define LCD_GRN_CHG                 0x070605000004
#define LCD_CYAN_CHG                0x070606000007
#define LCD_BLU_CHG                 0x070602000003
#define LCD_VIOL_CHG                0x070607000006
#define LCD_MAG_CHG                 0x070601000000
#define LCD_FADE_CHG                0x070608000009
#define LCD_ANIM_DEF_CHG            0x070609000008
#define LCD_SLD_FRATE_1             0x070400000003
#define LCD_SLD_FRATE_2             0x070400000102
#define LCD_SLD_FRATE_3             0x070400000201
#define LCD_SLD_FRATE_4             0x070400000300
#define LCD_SLD_FRATE_5             0x070400000407
#define LCD_SLD_FRATE_6             0x070400000506
#define LCD_SLD_FRATE_7             0x070400000605
#define LCD_SLD_FRATE_8             0x070400000704
#define LCD_SLD_SET_FRATE_1         0x010400000005
#define LCD_SLD_SET_FRATE_2         0x010400000104
#define LCD_SLD_SET_FRATE_3         0x010400000207
#define LCD_SLD_SET_FRATE_4         0x010400000306
#define LCD_SLD_SET_FRATE_5         0x010400000401
#define LCD_SLD_SET_FRATE_6         0x010400000500
#define LCD_SLD_SET_FRATE_7         0x010400000603
#define LCD_SLD_SET_FRATE_8         0x010400000702
#define LCD_SLD_RRATE_1             0x070401000002
#define LCD_SLD_RRATE_2             0x070401000103
#define LCD_SLD_RRATE_3             0x070401000200
#define LCD_SLD_RRATE_4             0x070401000301
#define LCD_SLD_RRATE_5             0x070401000406
#define LCD_SLD_RRATE_6             0x070401000507
#define LCD_SLD_RRATE_7             0x070401000604
#define LCD_SLD_RRATE_8             0x070401000705
#define LCD_SLD_SET_RRATE_1         0x010401000004
#define LCD_SLD_SET_RRATE_2         0x010401000105
#define LCD_SLD_SET_RRATE_3         0x010401000206
#define LCD_SLD_SET_RRATE_4         0x010401000307
#define LCD_SLD_SET_RRATE_5         0x010401000400
#define LCD_SLD_SET_RRATE_6         0x010401000501
#define LCD_SLD_SET_RRATE_7         0x010401000602
#define LCD_SLD_SET_RRATE_8         0x010401000703

// Some no-good, dirty, evil macros for simple option flag manipulation
#define SETFLAG( a, b )             ( ( a ) |= ( b ) )
#define CLRFLAG( a, b )             ( ( a ) &= ~( b ) )
#define TSTFLAG( a, b )             ( ( a ) & ( b ) )

#ifdef	__cplusplus
}
#endif

#endif


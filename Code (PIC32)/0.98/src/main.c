#include "../include/main.h"

/*** Pragmas for setting up PIC32 osillator and chip ***/
#pragma config POSCMOD      = HS        // Enable high speed oscillator module
#pragma config FNOSC        = PRIPLL    // use primary PLL
#pragma config FPLLIDIV     = DIV_2     // Divide 8MHz crystal clock by 2 for 4MHz to PLL input
#pragma config FPLLMUL      = MUL_20    // Multiply PLL by 15 (=80MHz)
#pragma config FPLLODIV     = DIV_2     // PLL output divide by 1 (=40MHZ Fcy)
#pragma config FPBDIV       = DIV_1     // Divide core clock by 1 for peripheral bus (=40MHz Fpb)
#pragma config FSOSCEN      = OFF
#pragma config FWDTEN       = OFF       // Disable Watchdog timer
#pragma config FCKSM        = CSDCMD
#pragma config CP           = OFF       // Disable code protect
#pragma config BWP          = OFF       // Disable boot write protect
#pragma config PWP          = OFF       // Disable program write Protect

// some important globals. 
volatile int cubeLayer = 0;
volatile uint16 bcmBitTicNext = 1;
volatile uint16 bcmBitTic = 1;
volatile uint16 bcmBitPos = 0;
volatile uint16 bcmBit = 0;
volatile uint32 fRateLast, rRateLast;
volatile struct animNode_t *head = NULL;
volatile struct animNode_t *curr = NULL;
volatile counter_t frmCtr;
volatile counter_t rgbCtr;
uint16 anIndex = 0;
unsigned long g_msCounter;
rgb_t g_colorBuffer[8][8][8];
anim_t anims[64] = { NULL };
anim_t *ptrAnim = NULL;

/*
===================
 Name

 Desc
===================
*/
void __ISR ( _TIMER_2_VECTOR, ipl1 ) TMR2IntHandler( void ) {
 //   mT4IntEnable(0);
    int16 x, y;
    LATBSET = INT_T2_TEST_PIN;   

     /* BCM stuff
     * 4 bits of color resolution (3375 colors) seems to work best at 40 Mhz
     * Adding the 5th bit results in a delay which causes noticeable flickering
     * due to the time spent processing a single layer. Still works better than
     * the previous versions, and with better accuracy. Might try to bum in a
     * 5th bit at some point just for the sake of doing so but not worth the
     * time right now. I also added some stuff elsewhere to automatically compensate for
     * color depths lower than 8 bits without having to rewrite the RGB color
     * handling logic for each possible case.
     */
    // bcmBitPos = [0...7]
    // When all BCM bits have been processed for this layer, move to the next one
    if ( bcmBitPos > CUBE_BCM_BITS ) {
        LATECLR = LAYER_ALL_Q;  // turn off all layer transistors
        // Increment the current layer of the cube.
        if ( cubeLayer++ > CUBE_Z_MAX ) {
            cubeLayer = 0;
        }
        // reset all when we've moved up one on the z-axis
        bcmBitPos = 0;
        bcmBitTic = 1;
        bcmBitTicNext = 1;
    }

    // set the current color weight bit to be evaluated from the frame buffer
    bcmBit = 1 << bcmBitPos;

    // if current BCM period is complete...
    if ( bcmBitTic == bcmBitTicNext ) {       
        // reset tick
        bcmBitTic = 1;
        // increment current BCM bit position
        bcmBitPos++;
        // double the next BCM tick count (period)
        bcmBitTicNext <<= 1;        

    } else {
        // otherwise, increment the tick count
        bcmBitTic++;
    }

    for ( y = CUBE_Y_MAX; y >= 0; y-- ) {
        for ( x = CUBE_X_MAX; x >= 0; x-- ) {
            // X-coord LEDs on odd-numbered Y-offsets are wired backwards (doh).
            // So this quick hack reverses the shift order on the X-axis on odd-numbered y-axes
            // This block can be removed if/when I ever correct the wiring
           if ( y % 2 != 0 ) {
               // calculate modified x-offset once to save a little time when indexing
                int _x = ( 7-x ); 

                if ( g_frameBuffer[_x][y][cubeLayer].r & bcmBit )
                    LATBSET = SR_SDI_R;

                if ( g_frameBuffer[_x][y][cubeLayer].g & bcmBit )
                    LATBSET = SR_SDI_G;

                if ( g_frameBuffer[_x][y][cubeLayer].b & bcmBit )
                    LATBSET = SR_SDI_B;                           
            } else {
                // If we're on an even-numbered Y-offset, shift the data normally.
                if ( g_frameBuffer[x][y][cubeLayer].r & bcmBit )
                    LATBSET = SR_SDI_R;

                if ( g_frameBuffer[x][y][cubeLayer].g & bcmBit )
                    LATBSET = SR_SDI_G;

                if ( g_frameBuffer[x][y][cubeLayer].b & bcmBit )
                    LATBSET = SR_SDI_B;
            }

            LATBSET = SR_CLK;
            LATBCLR = SR_CLK | SR_SDI_R | SR_SDI_G | SR_SDI_B;
        } // X axis
    } // Y axis
    //  asm volatile ("ei");
  
    int i;
    switch ( cubeLayer ) {
        case ( 0 ) :
//            LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q0;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
             break;
        case ( 1 ) :
 //           LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q1;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
             break;
        case ( 2 ) :
  //          LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q2;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
             break;
        case ( 3 ) :
 //           LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q3;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
             break;
        case ( 4 ) :
  //          LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q4;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
            break;
        case ( 5 ) :
 //           LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q5;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
            break;
        case ( 6 ) :
 //           LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q6;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
            break;
        case ( 7 ) :
  //          LATBSET = SR_OE;
            for ( i = 0; i < Q_DELAY; i++ ) {
                Nop();
            }
            LATESET = LAYER_Q7;
            LATBSET = SR_LE;
            LATBCLR = SR_LE | SR_OE;
            break;
        default:
            break;
    } // layer switch

    LATBCLR = INT_T2_TEST_PIN;
    mT2ClearIntFlag();
    mT4IntEnable( 1 );
    
} // End Timer2 interrupt

/*
===================
 Name

 Desc
===================
*/

void __ISR( _UART1_VECTOR, ipl2 ) IntUART1Handler( void ) {
    // on rx interrupt, read in the byte and build up the packet as required
    // TODO: store incoming bytes in circular buffer
    if ( mU1RXGetIntFlag() ) {
        // handle message immediately
        L_ReadByte();
        if ( uartBytesRead == 6 ) {
            // prevent T4 counter windups while handling message
            mT4IntEnable( 0 );
            L_MsgHandler( lcdReadBuffer );
            mT4IntEnable( 1 );
        }
        mU1RXClearIntFlag();
    }

    // Don't care about tx interrupt, return immediately
    if ( mU1TXGetIntFlag() ) {
        mU1TXClearIntFlag();
    }
    
} // end UART1 interrupt

/*
===================
 Name

 Desc
===================
*/
void __ISR ( _TIMER_4_VECTOR, ipl3 ) TMR4IntHandler( void ) {
    LATBSET = INT_T4_TEST_PIN;
    g_msCounter++;

    // Update SA peak detect counter
    if ( saPeakCtr.en  ) {
        if ( !saPeakCtr.dn ) {
            if ( ++saPeakCtr.acc == saPeakCtr.pre ) {
                saPeakCtr.dn = 1;
            }
        } else {
            saPeakCtr.acc = 0;
        }
    }

    // recalculate frame rate & rgb fade rate delay on change
    // if USE_GLOBAL flag is set, use rates from global options struct
    if ( TSTFLAG( xsig.AFLAGS, ANIM_USE_GLOBAL ) ) {
        if ( fRateLast != xsig.fRate ) {
            frmCtr.pre = 20000 / xsig.fRate;
            frmCtr.acc = 0;
            fRateLast = xsig.fRate;
        }

        if ( rRateLast != xsig.rRate ) {
            rgbCtr.pre = 20000 / xsig.rRate;
            rgbCtr.acc = 0;
            rRateLast = xsig.rRate;
        }
    // if USE_GLOBAL flag not set, use rates from this animation's options struct
    } else {
        if ( fRateLast != ptrAnim->rate ) {
            frmCtr.pre = 20000 / ptrAnim->rate;
            frmCtr.acc = 0;
            fRateLast = ptrAnim->rate;
        }

        if ( rRateLast != ptrAnim->rgbOpts.rate ) {
            rgbCtr.pre = 20000 / ptrAnim->rgbOpts.rate;
            rgbCtr.acc = 0;
            rRateLast = ptrAnim->rgbOpts.rate;
        }
    }

     // Update animation delay counter
    if ( frmCtr.en ) {
        if ( !frmCtr.dn ) {
            if ( ++frmCtr.acc == frmCtr.pre ) {
                frmCtr.dn = 1;
                frmCtr.acc = 0;
            }
        }
    }

    // Update RGB fade delay counter
    if ( rgbCtr.en ) {
        if ( !rgbCtr.dn ) {
            if ( ++rgbCtr.acc == rgbCtr.pre ) {
                rgbCtr.dn = 1;               
            }
        }

        // check RGB fade delay counter
        if ( rgbCtr.dn ) {
            if ( ++ptrAnim->rgbOpts.angle >= 360 ) {
                ptrAnim->rgbOpts.angle = 0;
            }
            rgbCtr.dn = 0;
            rgbCtr.acc = 0;
        }
    }

    // Get ADC samples for spectrum analyzer animation
    // Don't waste time doing this unless we're running this
    // animation
    if (ptrAnim->id == AN_SA) {
        S_GetADCSamples();
    } 

    LATBCLR = INT_T4_TEST_PIN;
    mT4ClearIntFlag();
} // End Timer4 interrupt

int main() {
    // Configure initial animation list
    I_InitAnim();
    // Configure general settings for chip
    I_InitConfig();
    // Configure LCP setup parameters
    I_InitLCD();
    // Launch main animation loop
    A_Animate();

    return ( EXIT_SUCCESS );
}   // End main





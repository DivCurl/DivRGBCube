#include "../include/init.h"

/*
===================
 I_InitADC

 Configure ADC settings
===================
*/
void I_InitADC( void ) {
    // disable the ADC before setup
    CloseADC10();
    // Use ground as negative reference for analog
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF );
    OpenADC10( ADC_CONFIG1, ADC_CONFIG2, ADC_CONFIG3, ADC_CONFIGPORT, ADC_CONFIGSCAN );
    // Setup for the ADC10.
    EnableADC10(); // Enables the ADC10.
    while ( ! mAD1GetIntFlag() ) {}
}

/*
===================
I_InitUART

 Configure UART settings
===================
*/
void I_InitUART( void ) {
    int PB_CLOCK = 40000000;
    int BAUD = 9600;
    // Setup and start UART1 (pins J14-1 & J14-3 on Max32 board)
    OpenUART1( UARTCONFIG1, UARTCONFIG2, PB_CLOCK/16/BAUD-1 );
    // Configure interrupt for UART1
    ConfigIntUART1( UART_INT_PR2 | UART_RX_INT_EN );
}

/*
===================
 I_InitAnim

 Configure preset animations list
===================
*/
void I_InitAnim( void ) {
    anims[0].id = AN_TEST;
    anims[0].rate = 10;
    anims[0].FLAGS = ANIM_FIRST_RUN;
    anims[0].its = 5;
    anims[0].rgbOpts.rate = 50;
    anims[0].rgbOpts.FLAGS = 0;

    anims[1].id = AN_SPLASH;
    anims[1].rate = 60;
    anims[1].FLAGS = ANIM_FIRST_RUN;
    anims[1].its = 1;
    anims[1].rgbOpts.rate = 20;
    anims[1].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;
   
    anims[2].id = AN_SINE_2D;
    anims[2].rate = 60;
    anims[2].FLAGS = ANIM_FIRST_RUN;
    anims[2].its = 256;
    anims[2].rgbOpts.rate = 30;
    anims[2].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;

    anims[3].id = AN_RAIN;
    anims[3].rate = 30;
    anims[3].FLAGS =  ANIM_FIRST_RUN;
    anims[3].its = 256;
    anims[3].rgbOpts.rate = 30;
    
    anims[4].id = AN_SINE_3D;
    anims[4].rate = 60;
    anims[4].FLAGS = ANIM_FIRST_RUN;
    anims[4].its = 256;
    anims[4].rgbOpts.rate = 30;
    anims[4].rgbOpts.FLAGS = 0;
    
    anims[5].id = AN_FACE_PALETTE_SWAP;
    anims[5].rate = 50;
    anims[5].FLAGS = ANIM_FIRST_RUN;
    anims[5].its = 512;
    anims[5].rgbOpts.rate = 30;
    anims[5].rgbOpts.FLAGS = 0;
    
    anims[6].id = AN_CUBE_FILL_DAZ;
    anims[6].rate = 250;
    anims[6].FLAGS = ANIM_FIRST_RUN;
    anims[6].its = 1;
    anims[6].rgbOpts.rate = 30;
    anims[6].rgbOpts.FLAGS = 0;
    
    anims[7].id = AN_HELIX;
    anims[7].rate = 60;
    anims[7].FLAGS = ANIM_FIRST_RUN;
    anims[7].its = 256;
    anims[7].rgbOpts.rate = 30;
    anims[7].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;
    
    anims[8].id = AN_DBL_HELIX;
    anims[8].rate = 60;
    anims[8].FLAGS = ANIM_FIRST_RUN;
    anims[8].its = 256;
    anims[8].rgbOpts.rate = 30;
    anims[8].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;
    
    anims[9].id = AN_DNA;
    anims[9].rate = 60;
    anims[9].FLAGS = ANIM_FIRST_RUN;
    anims[9].its = 512;
    anims[9].rgbOpts.rate = 30;
    anims[9].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;

    anims[10].id = AN_FIREWORKS;
    anims[10].rate = 40;
    anims[10].FLAGS = ANIM_FIRST_RUN | ANIM_RUN_FOREVER;
    anims[10].its = 512;
    anims[10].rgbOpts.rate = 30;
    anims[10].rgbOpts.FLAGS = 0;

    anims[32].id = AN_SA;
    anims[32].rate = 1;
    anims[32].FLAGS = ANIM_FIRST_RUN | ANIM_NO_DELAY | ANIM_RUN_FOREVER;
    anims[32].its = 512;
    anims[32].rgbOpts.rate = 10;
    anims[32].rgbOpts.FLAGS = RGB_VOX_OFF_FADEOUT;
  
    // Initialize animation pointer to first element
    ptrAnim = &anims[0];
    // Enable frame delay counter
    frmCtr.en = 1;
    rgbCtr.en = 1;
}

/*
===================
 Name

 Desc
===================
*/
void I_InitLCD( void ) {
    // Configure default non-momentary button settings on startup
    L_SendMessage( LCD_BTN_SET_PAUSE_OFF, 6 );   
    L_SendMessage( LCD_BTN_SET_GLOBAL_OFF, 6 );   
    L_SendMessage( LCD_SLD_SET_FRATE_3, 6 );    
    L_SendMessage( LCD_SLD_SET_RRATE_3, 6 );
}

/*
===================
 Name

 Desc
===================
*/
void I_InitConfig() {    
    // Optimize performance
    SYSTEMConfig( SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE );
    TRISB = 0xFF00;         // Set PORTB tristate pins to output (wired to shift register signals)
    TRISE = 0xFF00;         // Set PORTE tristate pins to output (layer control PNPs)
    // Start with all layers turned off
    LATECLR = LAYER_ALL_Q;
    // Clear all signal pins on PORTB on startup
    LATBCLR = SR_OE | SR_LE | SR_CLK | SR_SDI_R | SR_SDI_G | SR_SDI_B | INT_T2_TEST_PIN | INT_T4_TEST_PIN;
    INTEnableSystemMultiVectoredInt();
    // Disable JTAG shit
    mJTAGPortEnable( DEBUG_JTAGPORT_OFF );
    // Timer 2 used for main display refresh interrupt
    // Calibrated to ~20Khz frequency with oscope
    R_FillColorVector();

    I_InitUART();   
// #ifndef __DEBUG
    I_InitADC();   
// #endif

    srand( ReadADC10( 0 ) );    // seed PRNG

    // initial configuration setup for pre-configured animation list 
    OpenTimer2( T2_ON | T2_SOURCE_INT | T2_PS_1_1, 1500 );
    ConfigIntTimer2( T2_INT_ON | T2_INT_PRIOR_1 );
    // T4 used for ADC audio sampling & general timing
    // Calibrated to 20Khz frequency with oscope
    OpenTimer4( T4_ON | T4_SOURCE_INT | T4_PS_1_1, 2000 );
    ConfigIntTimer4( T4_INT_ON | T4_INT_PRIOR_3 );
}
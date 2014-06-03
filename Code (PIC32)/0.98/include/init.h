#ifndef INIT_H
#define	INIT_H

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

// define setup config #1 string for OpenUARTx peripheral macro
#define UARTCONFIG1 (           \
    UART_EN |                   \
    UART_IDLE_CON |             \
    UART_RX_TX |                \
    UART_DIS_WAKE |             \
    UART_DIS_LOOPBACK |         \
    UART_DIS_ABAUD |            \
    UART_NO_PAR_8BIT |          \
    UART_1STOPBIT |             \
    UART_IRDA_DIS |             \
    UART_DIS_BCLK_CTS_RTS |     \
    UART_NORMAL_RX |            \
    UART_BRGH_SIXTEEN           \
    )

// define setup config #2 for OpenUARTx peripheral macro
#define UARTCONFIG2 (           \
    UART_TX_PIN_LOW |           \
    UART_RX_ENABLE |            \
    UART_TX_ENABLE |            \
    UART_INT_TX |               \
    UART_INT_RX_CHAR |          \
    UART_ADR_DETECT_DIS |       \
    UART_RX_OVERRUN_CLEAR       \
    )

// Stuff for ADC config.
#define ADC_CONFIG1 (ADC_MODULE_ON | ADC_FORMAT_INTG32 | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON)
#define ADC_CONFIG2 (ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_ON | ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF)
#define ADC_CONFIG3 (ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15)
#define ADC_CONFIGPORT (ENABLE_AN15_ANA)
// Signal wired to AN15, skip all others for faster sampling.
#define ADC_CONFIGSCAN (                                \
    SKIP_SCAN_AN0 | SKIP_SCAN_AN1 | SKIP_SCAN_AN2 |     \
    SKIP_SCAN_AN3 | SKIP_SCAN_AN4 | SKIP_SCAN_AN5 |     \
    SKIP_SCAN_AN6 | SKIP_SCAN_AN7 | SKIP_SCAN_AN8 |     \
    SKIP_SCAN_AN9 | SKIP_SCAN_AN10 | SKIP_SCAN_AN11 |   \
    SKIP_SCAN_AN12 | SKIP_SCAN_AN13 | SKIP_SCAN_AN14    \
    )

void I_InitADC( void );
void I_InitUART( void );
void I_InitConfig( void );
void I_InitLCD( void );
void I_InitAnim( void );

#ifdef	__cplusplus
}
#endif

#endif	


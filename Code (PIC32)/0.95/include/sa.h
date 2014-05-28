#ifndef FFT_H
#define	FFT_H

#include <plib.h>       // PIC32 Peripheral Library.
#include <fftc.h>
#include "defs.h"
#include "types.h"
#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

// Definitions
#define SAMPLE_FREQ         20000
#define SAMPLE_MAX_VAL      450.0f
#define SAMPLE_MIN_VAL      60.0f
#define SAMPLE_DELTA        ((SAMPLE_MAX_VAL-SAMPLE_MIN_VAL)/7)
#define BAR_HOLD_DELAY      50
#define PEAK_HOLD_DELAY     40
#define COLOR_DEL           2500

// To modify the number of samples:
// #define fftc fft16c128 //from fftc.h, for N = 256 use fft16c256, for N = 1024 use fft16c1024
// #define N 128	// Also change the log2N variable below!!
#define fftc fft16c256 //from fftc.h, for N = 256 use fft16c256, for N = 1024 use fft16c1024
#define N 256	// Also change the log2N variable below!!

// To modify the sampling frequency
// #define SAMPLE_FREQ 10000

void S_GetSamples( void );
void S_Init( void );
void S_ComputeFFT( void );

#ifdef	__cplusplus
}
#endif

#endif


#include "../include/sa.h"

// NOTE - THIS NEEDS TO BE INCREMENTED IN THE ADC INPUT SAMPLING ROUTINE
int sampleIndex = 0; //keeps track of where we're putting our ADC reading
// this has to be changed for different values of N
// int log2N = 7; // log2(128) = 7
int log2N = 8; // log2(256) = 8
// int16c is a complex data type defined in fftc.h
// struct members are .re (int16) and .im (int16)
int16c sampleBuffer[N]; //initialize buffer to collect samples
int16c din[N]; // initialize buffer to hold old samples
int16c dout[N]; //holds computed FFT until transmission
int16c scratch[N];
long int singleSidedFFT[N];
long int freqVector[N];
int peak[8][8] = {NULL};     // stores the peak value of the x-y column
int peak_hold_delay[8][8] = {NULL};
int angle, angle_del, bar_del;

void saInit() {
    int i;
    /*  Getting compiler warnings when I use memset, not sure why...
    memset(freqVector, 0, sizeof(freqVector));
    memset(singleSidedFFT, 0, sizeof(singleSidedFFT));
    */

// zero the freqVector and singleSidedFFT
    for (i=0; i<N; i++) {

            freqVector[i] = 0;
            singleSidedFFT[i] = 0;
    }

    // generate frequency vector
    // this is the x-axis of your single sided fft
    for (i=0; i<N/2; i++) {

            freqVector[i] = i*(SAMPLE_FREQ/2)/((N/2) - 1);
    }

    angle_del = COLOR_DEL;
    bar_del = BAR_HOLD_DELAY;
}
  

void saGetSamples() {
    // read the ADC into the real parts
    sampleBuffer[sampleIndex].re = ReadADC10(0);
    // imaginary value starts at 0
    sampleBuffer[sampleIndex].im = 0;    
    // increment the sampleIndex
    if ( sampleIndex == ( N-1 ) ) {
        sampleIndex = 0;
    } else {
        sampleIndex++;
    }
    
}

void saComputeFFT() {
//    mT4IntEnable(0); //turns off interrupt while computing FFT
    
    // when using 256 samples, we measured this function to take about 500 microseconds
    // (not including the time to send rs232 data)

    int x, y, i, j;
   
    for (i = 0; i < N; i++) {

        if (i < sampleIndex) {
                // old chunk - sampleBuffer contains ADC samples
                din[i + (N - sampleIndex)] = sampleBuffer[i];
        }
        else {// i >= sampleIndex
                // new chunk - sampleBuffer contains ADC samples
                din[i - sampleIndex] = sampleBuffer[i];
        }
    }

    // load complex input data into din
    // result of FFT is stored in dout[]
    mips_fft16(dout, din, fftc, scratch, log2N);

    // compute single sided fft. Note that this is useful because the audio signals are symetical around the DC operating point
    // so this removes the negative half of the spectrum
    // G_aa(i) = 2 * S_aa(i) ;  from i = i to (N/2 - 1)
    for(i = 0; i < N/2; i++)
        singleSidedFFT[i] = 2 * ((dout[i].re*dout[i].re) + (dout[i].im*dout[i].im));

    // Once we have our FFT array filled out, we can draw the cube columns.
    // Loops through each column and converts 0-63 to x-y coords for
    // addressing of the cube buffer

    sa_peak_ctr.pre = D_SetCtrmS(30);
    sa_peak_ctr.en = 1;

    // decrement the peak on the columns when the peak delay hits zero.
    // Should be fast but not too fast for good visuals
    if (sa_peak_ctr.dn) {
        for (x = 0; x < 8; x++) {
            for (y = 0; y < 8; y++) {
                if (peak_hold_delay[x][y] == 0) {
                    if ((peak[x][y]-1) > 0)
                        peak[x][y]--;
                    else
                        peak[x][y] = 0;
                }
            }
        }   // for loop
        sa_peak_ctr.dn = 0;
    }

    // Converts linear array elements (1-64) into x-y
    // coordinates for cube column addressing
    for (j = 1; j <= 64; j++) {
        // y = 0, x = 0-7
        if (j <= 8) {
            y = 0;
            x = j-1;
        }
        // y = 1, x = 0-7
        if (j > 8 && j <= 16) {
            y = 1;
            x = 7 - (16-j);
        }
        // y = 2, x = 0-7
        if (j > 16 && j <= 24) {
            y = 2;
            x = 7 - (24-j);
        }
        // y = 3, x = 0-7
        if (j > 24 && j <= 32) {
            y = 3;
            x = 7 - (32-j);
        }
        // y = 4, x = 0-7
        if (j > 32 && j <= 40) {
            y = 4;
            x = 7 - (40-j);
        }
        // y = 5, x = 0-7
        if (j > 40 && j <= 48) {
            y = 5;
            x = 7 - (48-j);
        }
        // y = 6, x = 0-7
        if (j > 48 && j <= 56) {
            y = 6;
            x = 7 - (56-j);
        }
        // y = 7, x = 0-7
        if (j > 56 && j <= 64) {
            y = 7;
            x = 7 - (64-j);
        }
        // if the peak hold is still active for this column, delay it
        if (peak_hold_delay[x][y] > 0)
            peak_hold_delay[x][y]--;

        // quick and dirty delay for color fading
        if (angle_del > 0)
            angle_del--;

        else  {
            angle++;
            if (angle > 360)
                angle = 0;
            angle_del = COLOR_DEL;
        }

        rgb_t color;
        color = R_GetAngleColor(angle);

        // First, draw the instananeous spectral content (0-5Khz)
        // The peak of each column is saved and used in the next loop
        if (singleSidedFFT[j] <= SAMPLE_MIN_VAL ) {
            D_ClrCol(x, y);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL ) {
            if (peak[x][y] < 1) {
                peak[x][y] = 1;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,1,color);
        }
        
        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 1*SAMPLE_DELTA) {
            if (peak[x][y] < 2) {
                peak[x][y] = 2;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,2,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 2*SAMPLE_DELTA) {
            if (peak[x][y] < 3) {
                peak[x][y] = 3;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,3,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 3*SAMPLE_DELTA) {
            if (peak[x][y] < 4) {
                peak[x][y] = 4;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,4,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 4*SAMPLE_DELTA) {
            if (peak[x][y] < 5) {
                peak[x][y] = 5;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,5,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 5*SAMPLE_DELTA) {
            if (peak[x][y] < 6) {
                peak[x][y] = 6;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,6,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 6*SAMPLE_DELTA) {
            if (peak[x][y] < 7) {
                peak[x][y] = 7;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,7,color);
        }

        if (singleSidedFFT[j] > SAMPLE_MIN_VAL + 7*SAMPLE_DELTA) {
            if (peak[x][y] < 8) {
                peak[x][y] = 8;
                peak_hold_delay[x][y] = PEAK_HOLD_DELAY;
            }
            D_FillCol(x,y,8,color);
        }

        // Draw the column's peak level voxel
        if ((peak[x][y]-1) >= 0)
            V_Set(x, y, (peak[x][y]-1), R_SetColorRGB(RGB_RED));
    } // end for loop

    // Output buffer to cube display after drawing all columns
    D_FBUpdate();
 
}
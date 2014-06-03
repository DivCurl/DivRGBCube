/******************************************************************************
 * Serial data exchange interface driver
 * TODO: interace with Rapsberry Pi to receive mode settings
 * which will essentially be pushed into the global variables
 * defined in main.h. The Pi will run a server which a user can access
 * via a webpage or the like to be able to make runtime changes to the animation
 * controls (speed, color, current animation, how many iterations, etc)
 *
 * Written by Mike Dillmann
 *
 ******************************************************************************/
#include "../include/serial.h"

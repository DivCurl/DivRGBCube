#ifndef SERIAL_H
#define	SERIAL_H

#include "types.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define SERIAL_INIT         0x01
#define SERIAL_OK           0x02
#define SERIAL_NO_DATA      0x04

uint8 serial_link_stat;

#ifdef	__cplusplus
}
#endif

#endif	


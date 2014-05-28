#ifndef LCD_H
#define	LCD_H

#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

int uartBytesRead = 0;
bool isReadPacket;
uint64_t lcdReadBuffer;

inline void L_MsgHandler( uint64_t packet );
inline uint64_t L_ReadByte();
void L_SendMessage( const uint64_t buffer, int len );


#ifdef	__cplusplus
}
#endif

#endif


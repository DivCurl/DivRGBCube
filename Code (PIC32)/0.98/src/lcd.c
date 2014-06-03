#include "../include/lcd.h"

/*
===================
 L_MsgHandler

 Handles messages received from uLCD-70DT touchscreen via UART
 ISR.
===================
*/
inline void L_MsgHandler( uint64_t packet ) {
    switch ( packet ) {
        case ( LCD_BTN_PAUSE_ON ) :
           frmCtr.en = 0;
            rgbCtr.en = 0;
            break;

        case ( LCD_BTN_PAUSE_OFF ) :
            frmCtr.en = 1;
            rgbCtr.en = 1;
            break;

        case ( LCD_BTN_RUN_FOREVER_ON ) :
            SETFLAG( ptrAnim->FLAGS, ANIM_RUN_FOREVER );
            break;

        case ( LCD_BTN_RUN_FOREVER_OFF ) :
            CLRFLAG( ptrAnim->FLAGS, ANIM_RUN_FOREVER );
            break;

        case ( LCD_BTN_GLOBAL_ON ) :
            SETFLAG( xsig.AFLAGS, ANIM_USE_GLOBAL );
            // set initial rates if not yet set
            if ( xsig.fRate == 0 || xsig.rRate == 0 ) {
                xsig.fRate = ptrAnim->rate;
                xsig.rRate = ptrAnim->rgbOpts.rate;
            }
            break;

        case ( LCD_BTN_GLOBAL_OFF ) :
            CLRFLAG( xsig.AFLAGS, ANIM_USE_GLOBAL );
            break;

        case ( LCD_RED_CHG  ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_RED );
            break;

        case ( LCD_ORG_CHG ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_ORANGE );
            break;

        case ( LCD_YEL_CHG ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_YELLOW );
            break;

        case ( LCD_GRN_CHG  ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_GREEN );
            break;

        case ( LCD_CYAN_CHG ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_CYAN );
            break;

        case ( LCD_BLU_CHG ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_BLUE );
            break;

        case ( LCD_VIOL_CHG  ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_VIOLET );
            break;

        case ( LCD_MAG_CHG ) :
            xsig.rgbMode = RGB_MODE_SOLID;
            xsig.color = R_SetColorRGB( RGB_MAGENTA );
            break;

        case ( LCD_FADE_CHG ) :
            xsig.rgbMode = RGB_MODE_FADE;
            break;

        case ( LCD_ANIM_DEF_CHG ) :
            xsig.rgbMode = RGB_MODE_DEF;
            break;

        case ( LCD_BTN_AN_NEXT ) :
            SETFLAG( xsig.AFLAGS, ANIM_CHANGE_PENDING );
            if ( ptrAnim->id == AN_SA ) {
                xsig.idNext = xsig.idLast;
            } else {
                xsig.idNext = anims[anIndex + 1].id;
            }
            break;

        case ( LCD_BTN_AN_PREV ) :
            SETFLAG( xsig.AFLAGS, ANIM_CHANGE_PENDING );
            if ( ptrAnim->id == AN_SA ) {
                xsig.idNext = xsig.idLast;
            } else {
                xsig.idNext = anims[anIndex - 1].id;
            }
            break;
            
        case ( LCD_SLD_FRATE_1 ) :
            xsig.fRate = 10;
            break;

        case ( LCD_SLD_FRATE_2 ) :
            xsig.fRate = 30;
            break;

        case ( LCD_SLD_FRATE_3  ) :
            xsig.fRate = 60;
            break;

        case ( LCD_SLD_FRATE_4 ) :
            xsig.fRate = 90;
            break;

        case ( LCD_SLD_FRATE_5 ) :
            xsig.fRate = 120;
            break;

        case ( LCD_SLD_FRATE_6  ) :
            xsig.fRate = 150;
            break;

        case ( LCD_SLD_FRATE_7 ) :
            xsig.fRate = 180;
            break;

        case ( LCD_SLD_FRATE_8 ) :
            xsig.fRate = 210;
            break;

        case ( LCD_SLD_RRATE_1 ) :
            xsig.rRate = 10;
            break;

        case ( LCD_SLD_RRATE_2 ) :
            xsig.rRate = 30;
            break;

        case ( LCD_SLD_RRATE_3  ) :
            xsig.rRate = 60;
            break;

        case ( LCD_SLD_RRATE_4 ) :
            xsig.rRate = 90;
            break;

        case ( LCD_SLD_RRATE_5 ) :
            xsig.rRate = 120;
            break;

        case ( LCD_SLD_RRATE_6  ) :
            xsig.rRate = 150;
            break;

        case ( LCD_SLD_RRATE_7 ) :
            xsig.rRate = 180;
            break;

        case ( LCD_SLD_RRATE_8 ) :
            xsig.rRate = 210;
            break;

        case ( LCD_BTN_AN_SWITCH_SA ) :
            SETFLAG( xsig.AFLAGS, ANIM_CHANGE_PENDING );
            xsig.idLast = ptrAnim->id;
            xsig.idNext = AN_SA;
            break;

        default :
            break;
    }
    
    lcdReadBuffer = 0;
}

/*
===================
 L_ReadByte

 Reads and formats data bytes received from touchscreen into message packets which can
 then be handled by the message handler.

 Function is called from UART ISR.
===================
*/
inline uint64_t L_ReadByte() {
    // get most recent byte out of UART buffer
    uint64_t rxByte = ReadUART1();

    // if we get an ACK or NAK response, return immediately. Note this would only happen following a send message
    if (uartBytesRead == 0 && ((rxByte == LCD_ACK) || (rxByte == LCD_NAK))) {
        return rxByte;
    }

    // Was a full data packet previously read? Reset buffers if so.
    if ( uartBytesRead == 6 ) {
        isReadPacket = 0;
        uartBytesRead = 0;
        lcdReadBuffer = 0;
    }

    // if first byte read and is respone to query (byte 0 value is 0x1 or 0x7), flag it as packet header
    if ((uartBytesRead == 0) && ((rxByte == 0x01) ||  (rxByte == 0x07))) {
        isReadPacket = TRUE;
    }

    // continue reading packets after packet header found...
    if (isReadPacket) {
        lcdReadBuffer = (lcdReadBuffer << 8) | rxByte;
        uartBytesRead++;
    }

    return rxByte;
}

/*
===================
 Name

 Desc
===================
*/
void L_SendMessage(const uint64_t msg, int len) {

    // sends out packet bytes on UART depending on
    // packet buffer length (between 1 and 6 bytes)
    if ( len >= 6 ) {
        putcUART1( (char)( ( msg >> 40 ) & 0xff ) );
    }
    if ( len >= 5 ) {
        putcUART1( (char)( ( msg >> 32 ) & 0xff ) );
    }
    if ( len >= 4 ) {
        putcUART1( (char)( ( msg >> 24 ) & 0xff ) );
    }
    if ( len >= 3 ) {
        putcUART1( (char)( ( msg >> 16 ) & 0xff ) );
    }
    if ( len >= 2 ) {
        putcUART1( (char)( ( msg >> 8 ) & 0xff ) );
    }
    if ( len >= 1 ) {
        putcUART1( (char)( ( msg  ) & 0xff ) );
    }

    D_Delayms( 50 );
}




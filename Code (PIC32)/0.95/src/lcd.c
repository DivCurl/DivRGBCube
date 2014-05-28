#include "../include/lcd.h"

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

        case ( LCD_BTN_STBY_ON ) :
            break;

        case ( LCD_BTN_STBY_OFF ) :
            break;

        case ( LCD_BTN_GLOBAL_ON ) :
            SETFLAG( gblOpts.AFLAGS, ANIM_USE_GLOBAL );
            // set initial rates if not yet set
            if ( gblOpts.fRate == 0 || gblOpts.rRate == 0 ) {
                gblOpts.fRate = ptrAnim->rate;
                gblOpts.rRate = ptrAnim->rgbOpts.rate;
            }
            break;

        case ( LCD_BTN_GLOBAL_OFF ) :
            CLRFLAG( gblOpts.AFLAGS, ANIM_USE_GLOBAL );
            break;

        case ( LCD_RED_CHG  ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_RED );
            break;

        case ( LCD_ORG_CHG ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_ORANGE );
            break;

        case ( LCD_YEL_CHG ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_YELLOW );
            break;

        case ( LCD_GRN_CHG  ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_GREEN );
            break;

        case ( LCD_CYAN_CHG ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_CYAN );
            break;

        case ( LCD_BLU_CHG ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_BLUE );
            break;

        case ( LCD_VIOL_CHG  ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_VIOLET );
            break;

        case ( LCD_MAG_CHG ) :
            gblOpts.rgbMode = RGB_MODE_SOLID;
            gblOpts.color = R_SetColorRGB( RGB_MAGENTA );
            break;

        case ( LCD_FADE_CHG ) :
            gblOpts.rgbMode = RGB_MODE_FADE;
            break;

        case ( LCD_ANIM_DEF_CHG ) :
            gblOpts.rgbMode = RGB_MODE_DEF;
            break;

        case ( LCD_ANIM_SKIP_CHG ) :
            ptrAnim->it = 0;
            if ( ++anIndex > 9) {
                anIndex = 1;
            }
            ptrAnim = &anims[anIndex];
            break;

        case ( LCD_SLD_FRATE_1 ) :
            gblOpts.fRate = 10;
            break;

        case ( LCD_SLD_FRATE_2 ) :
            gblOpts.fRate = 30;
            break;

        case ( LCD_SLD_FRATE_3  ) :
            gblOpts.fRate = 60;
            break;

        case ( LCD_SLD_FRATE_4 ) :
            gblOpts.fRate = 90;
            break;

        case ( LCD_SLD_FRATE_5 ) :
            gblOpts.fRate = 120;
            break;

        case ( LCD_SLD_FRATE_6  ) :
            gblOpts.fRate = 150;
            break;

        case ( LCD_SLD_FRATE_7 ) :
            gblOpts.fRate = 180;
            break;

        case ( LCD_SLD_FRATE_8 ) :
            gblOpts.fRate = 210;
            break;

        case ( LCD_SLD_RRATE_1 ) :
            gblOpts.rRate = 10;
            break;

        case ( LCD_SLD_RRATE_2 ) :
            gblOpts.rRate = 30;
            break;

        case ( LCD_SLD_RRATE_3  ) :
            gblOpts.rRate = 60;
            break;

        case ( LCD_SLD_RRATE_4 ) :
            gblOpts.rRate = 90;
            break;

        case ( LCD_SLD_RRATE_5 ) :
            gblOpts.rRate = 120;
            break;

        case ( LCD_SLD_RRATE_6  ) :
            gblOpts.rRate = 150;
            break;

        case ( LCD_SLD_RRATE_7 ) :
            gblOpts.rRate = 180;
            break;

        case ( LCD_SLD_RRATE_8 ) :
            gblOpts.rRate = 210;
            break;

        default :
            break;

    }
    
    lcdReadBuffer = 0;
}

inline uint64_t L_ReadByte() {
    uint64_t rxByte = ReadUART1();

    // Was a full packet previously read? Reset buffers if so.
    if (uartBytesRead == 6) {
        isReadPacket = 0;
        uartBytesRead = 0;
        lcdReadBuffer = 0;
    }

    // if we get an ACK or NAK response, return immediately. Note this would only happen following a send message
    if (uartBytesRead == 0 && ((rxByte == LCD_ACK) || (rxByte == LCD_NAK))) {
        return rxByte;
    }

    // if first byte read and is respone to query (byte 0 is '1' or '7'), flag it as packet header
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

void L_SendMessage(const uint64_t buffer, int len) {
    int delay = 0;

    // sends out packet bytes on UART depending on
    // packet buffer length (between 1 and 6 bytes)
    // - transmit message in big-endian format
    if (len >= 6)
        putcUART1((char)((buffer >> 40) & 0xff));
    if (len >= 5)
        putcUART1((char)((buffer >> 32) & 0xff));
    if (len >= 4)
        putcUART1((char)((buffer >> 24) & 0xff));
    if (len >= 3)
        putcUART1((char)((buffer >> 16) & 0xff));
    if (len >= 2)
        putcUART1((char)((buffer >> 8) & 0xff));
    if (len >= 1)
        putcUART1((char)((buffer ) & 0xff));
}

/*
void L_Test() {

    // A full response packet has been received from the touchscreen
    if (uartBytesRead == 6) {
        if (lcdReadBuffer == lcdMsgLUT[LCD_ROCKER_CHG_ON]) {
            LATBCLR = SR_OE;
            lcdReadBuffer = 0;
        }


        if (lcdReadBuffer == lcdMsgLUT[LCD_ROCKER_CHG_OFF]) {
            lcdReadBuffer = 0;
            LATBSET = SR_OE;
        }

        uint8 x, y, z;
        if (lcdReadBuffer == lcdMsgLUT[LCD_RED_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_RED));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        }

        if (lcdReadBuffer == lcdMsgLUT[LCD_ORG_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_ORANGE));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_YEL_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_YELLOW));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_GRN_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_GREEN));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_CYAN_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_CYAN));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_BLU_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_BLUE));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_VIOL_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_VIOLET));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        if (lcdReadBuffer == lcdMsgLUT[LCD_MAG_CHG]) {
            lcdReadBuffer = 0;
            for (z = 0; z <= CUBE_Z_MAX; z++) {
                for (y = 0; y <= CUBE_Y_MAX; y++) {
                    for (x = 0; x <= CUBE_X_MAX; x++) {
                        V_Set(x,y,z,R_SetColorRGB(RGB_MAGENTA));
                    } // x
                } // y
            } // z
             D_FBUpdate();
        } // if

        // Get slider value on slider event change  - and flexing my bit twiddling muscles a bit...
        // Bytes 4 and 5 should equal 0x0704 to indicate slider change
        if (((((lcdReadBuffer >> 32) | 0xFF00) ^ 0xFF00) == 0x04)) {
            // mask out byte 1 to get value of slider
            uint64_t byte_mask = (((lcdReadBuffer >> 8) | 0xFFFFFFFF00) ^ 0xFFFFFFFF00);
            if (byte_mask == 0x00)
                Nop();
            if (byte_mask < 0x01)
                D_ClrPlaneZ(1);
            if (byte_mask < 0x02)
                D_ClrPlaneZ(2);
            if (byte_mask < 0x03)
                D_ClrPlaneZ(3);
            if (byte_mask < 0x04)
                D_ClrPlaneZ(4);
            if (byte_mask < 0x05)
                D_ClrPlaneZ(5);
            if (byte_mask < 0x06)
                D_ClrPlaneZ(6);
            if (byte_mask < 0x07)
                D_ClrPlaneZ(7);

            lcdReadBuffer = 0;
            D_FBUpdate();
        } // slider change        
    } // 6 bytes read   
}
*/




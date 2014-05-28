#ifndef FONT_H
#define FONT_H

#ifdef	__cplusplus
extern "C" {
#endif

void F_SendText( const char *, rgb_t color );
void F_GetPath(uchar path, uchar *destination, int length);
void F_Getchar (char chr, uchar *dst);
void F_GetBitmap (char bitmap, uchar *dst);
uint8 F_GetBitmapPixel (char bitmap, char x, char y);

#ifdef	__cplusplus
}
#endif

#endif

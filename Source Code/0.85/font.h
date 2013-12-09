#ifndef FONT_H
#define FONT_H

#ifdef	__cplusplus
extern "C" {
#endif

void Font_Getchar(char chr, unsigned char dst[5]);
void Font_Getchar_Flipped(char chr, unsigned char dst[5]);
void Font_Getpath (unsigned char path, unsigned char *destination, int length);
void Font_Getbitmap (char bitmap, unsigned char dst[8]);
unsigned char Font_Get_Bitmap_Pixel ( char bitmap, char x, char y);

#ifdef	__cplusplus
}
#endif

#endif

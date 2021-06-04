#ifndef ___FUNCTIONS____
#define ___FUNCTIONS____

#include <stdio.h>
#include "bmp.h"

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, GRAY_VALUE img[height][width], GRAY_VALUE image_new[height][new_width]);
void WelcomeMsg();
void ReadFile(FILE* src, BITMAP_HEADER *fHeader, INFO_HEADER *fInfo);
void NewHeader(BITMAP_HEADER *fHeader, INFO_HEADER *fInfo, DWORD new_width);
void WriteFile(FILE **dst, BITMAP_HEADER fHeader, INFO_HEADER fInfo, BYTE colorTable[]);
void Dither(DWORD height, DWORD width, GRAY_VALUE img[height][width], GRAY_VALUE img_dithered[height][width]);

#endif
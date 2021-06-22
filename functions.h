#ifndef ___FUNCTIONS____
#define ___FUNCTIONS____

#include "bmp.h"

void WelcomeMsg();

void DitherOpt();

void ReadFile(FILE* src, BITMAP_HEADER *fHeader, INFO_HEADER *fInfo);

void NewHeader(BITMAP_HEADER *fHeader, INFO_HEADER *fInfo, DWORD new_width);

void WriteHeader(FILE **dst, BITMAP_HEADER fHeader, INFO_HEADER fInfo, BYTE colorTable[]);

void Dither(DWORD height, DWORD width, BYTE img[height][width], BYTE img_dithered[height][width]);

void WriteData(DWORD height, DWORD new_width, FILE* dst, BYTE img_new[][new_width], DWORD padding);

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, BYTE img[height][width], BYTE image_new[height][new_width]);

char* getFileNameFromPath(char* path);

void delay(int number_of_seconds);

#endif
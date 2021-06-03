#ifndef ___FUNCTIONS____
#define ___FUNCTIONS____

#include "bmp.h"

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, GRAY_VALUE img[height][width], GRAY_VALUE image_new[height][new_width]);
void FileCheck(BITMAP_HEADER fHeader, INFO_HEADER fInfo);

#endif
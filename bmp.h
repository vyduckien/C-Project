#ifndef ____bmp_____
#define ____bmp_____

#include <stdint.h>
#pragma pack(1)

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct 
{
    WORD Signature;
    DWORD fSize;
    WORD fReserved_1;
    WORD fReserved_2;
    DWORD fOffset;
} BITMAP_HEADER;

typedef struct 
{
    DWORD Size;
    LONG  Width;
    LONG  Height;
    WORD  Planes;
    WORD  BitsPerPixel;
    DWORD Compression;
    DWORD imgSize;
    LONG  ResX;
    LONG  ResY;
    DWORD Color;
    DWORD ImpColor;
} INFO_HEADER;

typedef struct
{
    BYTE rgbtBlue;
    // BYTE rgbtGreen;
    // BYTE rgbtRed;
} RGBTRIPLE;

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, RGBTRIPLE img[height][width], RGBTRIPLE image_new[height][new_width]);

#endif
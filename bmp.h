#ifndef ____bmp_____
#define ____bmp_____

#include <stdint.h>
#pragma pack(1)         //avoid padding between struct members

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
    BYTE Val;
} GRAY_VALUE;

#endif
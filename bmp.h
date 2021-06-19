#ifndef ____bmp_____
#define ____bmp_____

#include <stdint.h>
#pragma pack(1)         //avoid padding between struct members

typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct BITMAP_HEADER
{
    WORD Signature;         //The header field used to identify the BMP file
    DWORD Size;            //The size of the BMP file in bytes
    WORD Reserved_1;       //Reserved
    WORD Reserved_2;       //Reserved
    DWORD Offset;          //The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
} BITMAP_HEADER;

typedef struct INFO_HEADER
{
    DWORD Size;         //Size of header (40 bytes)
    LONG  Width;        //Bitmap width in pixels (signed integer)
    LONG  Height;       //Bitmap height in pixels (signed integer)
    WORD  Planes;       //Number of color planes (must be 1)
    WORD  BitsPerPixel; //Number of bits per pixel. Typical values are 1, 4, 8, 16, 24 and 32
    DWORD Compression;  //Compression method being used
    DWORD imgSize;      //Image size
    LONG  ResX;         //Horizontal resolution (pixel per metre, signed integer)
    LONG  ResY;         //Bertical resolution of the image. (pixel per metre, signed integer)
    DWORD Color;        //Number of colors in the color palette, or 0 to default to 2n
    DWORD ImpColor;     //Number of important colors used, or 0 when every color is important; generally ignored
} INFO_HEADER;

#endif
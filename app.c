/**
 * Write a program to convert the image file .BMP of 256 grey levels 
 * to binary image .BMP for the data section in the image file with the rule:
 *                      gray levels           binary      
 *                        0 – 127               0
 *                      128 – 255               1
 * 
 * Author: Vy Duc Kien
 * ID: 1951010
 * Date: 24/05/2021
 */
    
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "bmp.h"

int main()
{
    FILE *src, *dst;
    src = fopen("Resources/lena.bmp", "rb");         //bmp is binary file, therefore use "rb" permission
    dst = fopen("Resources/output.bmp", "wb");

    BITMAP_HEADER fHeader;
    INFO_HEADER   fInfo;

    //read file headers
    fseek(src, 0, SEEK_SET);
    fread(&fHeader, sizeof(BITMAP_HEADER), 1, src);
    fread(&fInfo, sizeof(INFO_HEADER), 1, src);

    DWORD width = (fInfo.Width % 2 == 1)? (fInfo.Width + 1) : fInfo.Width;  //width must be an even value
    DWORD height = fInfo.Height;
    DWORD padding = (4 - (width * sizeof(GRAY_VALUE)) % 4) % 4;
    DWORD new_width = width/8;
    // printf("Height: %d\n", height);
    // printf("Width: %d\n", width);
    // printf("New Width: %d\n", new_width);

    //read pixel data
    fseek(src, fHeader.fOffset, SEEK_SET);
    GRAY_VALUE (*image)[width] = (GRAY_VALUE *)malloc(height * sizeof(*image));
    GRAY_VALUE (*image_new)[new_width] = (GRAY_VALUE *)calloc(height, sizeof(*image));    //allocate memory and initialize all with 0

    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(GRAY_VALUE), width, src);
        fseek(src, padding,  SEEK_CUR);                     //skip over padding
    }

    //Convert to binary
    BinaryConvert(height, width, new_width, image, image_new);

    BYTE colorTable[8] = {0, 0, 0, 0, 255, 255, 255, 0};
    //BYTE colorTable[8] = {255,255,255,0,0,0,0,0};          //Endianness of BMP files is Little-Endian,
                                                             //thus the inverse arrangement
    //Modify header information
    fHeader.Signature = 0x4D42;
    fHeader.fOffset = 62;
    fHeader.fSize = new_width * fInfo.Height + 62;
    fInfo.BitsPerPixel = 1;
    fInfo.imgSize = new_width * fInfo.Height;
    fInfo.Color = 2;

    //Write file header
    fseek(dst, 0, SEEK_SET);
    // printf("dst after seek: %lu", ftell(dst));
    fwrite(&fHeader, sizeof(BITMAP_HEADER), 1, dst);
    fwrite(&fInfo, sizeof(INFO_HEADER), 1, dst);
    // printf("\ncp after reading bHeader: %lu\n", ftell(dst));
    fwrite(colorTable, sizeof(BYTE), 8, dst);
    // printf("dst after writing color palette: %lu\n", ftell(dst));

    //write pixel data to output
    fseek(dst, fHeader.fOffset, SEEK_SET);
    printf("dst before writing: %lu\n", ftell(dst));
    for (int i = 0; i < height; i++)
    {
        fwrite(image_new[i], sizeof(GRAY_VALUE), new_width, dst);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, dst);
        }
    }

    //Complete and close all files
    fclose(dst);
    fclose(src);   
}
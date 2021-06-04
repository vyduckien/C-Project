#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bmp.h"
#include "functions.h"

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, GRAY_VALUE img[height][width], GRAY_VALUE image_new[height][new_width])
{
    //convert grey levels to black and white color
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (img[i][j].Val <= 127 && img[i][j].Val >= 0)
                img[i][j].Val = 0;
            else if (img[i][j].Val <= 255 && img[i][j].Val >= 128)
                img[i][j].Val = 0x1;
        }
    }
    
    //squeeze 8 bytes of 0 and 1 into one byte using bit shifting
    for (int i = 0; i < height; i++)
    {
        int k = 0;
        int count = 0;
        int shift = 7;
        int j = 0;
        for (j = 0; j < new_width; j++)
        {
            for (k = count; k < count + 8; k++)
            {   
                image_new[i][j].Val += img[i][k].Val << (shift - k);
                count = k;
                if (count % width == shift)
                {
                    count++;
                    shift = shift + 8;
                    break;
                }
            }
        }
    }
}

void WelcomeMsg()
{
    printf("=========================================================\n");
    printf("\t\tWELCOME TO THE C PROJECT.\n");
    printf("=========================================================");
    printf("\n0 - Begin the program.");
    printf("\n1 - Exit the program");
    printf("\n2 - Further information.");
}

void ReadFile(FILE* src, BITMAP_HEADER *fInfo, INFO_HEADER *fHeader)
{
    fseek(src, 0, SEEK_SET);
    fread(fInfo, sizeof(BITMAP_HEADER), 1, src);
    fread(fHeader, sizeof(INFO_HEADER), 1, src);
}

void NewHeader(BITMAP_HEADER *fHeader, INFO_HEADER *fInfo, DWORD new_width)
{
    (*fHeader).Signature = 0x4D42;
    (*fHeader).fOffset = 62;
    (*fHeader).fSize = new_width * (*fInfo).Height + 62;
    (*fInfo).BitsPerPixel = 1;
    (*fInfo).imgSize = new_width * (*fInfo).Height;
    (*fInfo).Color = 2;
}

void WriteFile(FILE **dst, BITMAP_HEADER fHeader, INFO_HEADER fInfo, BYTE colorTable[])
{
    fseek(*dst, 0, SEEK_SET);
    fwrite(&fHeader, sizeof(BITMAP_HEADER), 1, *dst);
    fwrite(&fInfo, sizeof(INFO_HEADER), 1, *dst);
    fwrite(colorTable, sizeof(BYTE), 8, *dst);
}

void Dither(DWORD height, DWORD width, GRAY_VALUE img[height][width], GRAY_VALUE img_dithered[height][width])
{
    int error;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img_dithered[i][j].Val = img[i][j].Val;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE old = img_dithered[i][j].Val;
            img_dithered[i][j].Val = (img_dithered[i][j].Val < 128) ? 0 : 255;
            error = old - img_dithered[i][j].Val;
            if (i + 1 < height)
            {
                if (j - 1 > 0)
                {
                    img_dithered[i + 1][j - 1].Val = img_dithered[i + 1][j - 1].Val + round((float)(error * 3)/ 16);
                }
                
                img_dithered[i + 1][j    ].Val = img_dithered[i + 1][j    ].Val + round((float)(error * 5)/ 16);
                if (j + 1 < width)
                {
                    img_dithered[i + 1][j + 1].Val = img_dithered[i + 1][j + 1].Val + round((float)(error * 1)/ 16);
                    img_dithered[i    ][j + 1].Val = img_dithered[i    ][j + 1].Val + round((float)(error * 7)/ 16);
                }
                
            }
                
                
                
                
        }
            
    }

    

    // for (int i = 0; i < width; i++)
    // {
    //    printf("%d ",  img[0][i].Val);
    // }
    // printf("\nAfter dithering:\n");
    // for (int i = 0; i < width; i++)
    // {
    //    printf("%d ",  img_dithered[0][i].Val);
    // }
    int count = 0;
    for (int i = 0; i < width; i++)
    {
       printf("%X ",  img[0][i].Val);
       count ++;
    }
    printf("\nNumber of elements: %d\n", count);

    count = 0;
    printf("After dithering:\n");
    for (int i = 0; i < width; i++)
    {
       printf("%X ",  img_dithered[1][i].Val);
       count++;
    }
    printf("\nNumber of elements: %d\n", count);
}

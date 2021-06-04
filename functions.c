#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "bmp.h"
#include "functions.h"

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, BYTE img[height][width], BYTE image_new[height][new_width])
{
    //convert grey levels to black and white color
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img[i][j] = (img[i][j] < 128) ? 0x00 : 0x01;
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
                image_new[i][j] += img[i][k] << (shift - k);
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

void Dither(DWORD height, DWORD width, BYTE img[height][width], BYTE img_dithered[height][width])
{
    int error;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            img_dithered[i][j] = img[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE old = img_dithered[i][j];
            // printf("old: %d\n", old);
            img_dithered[i][j] = (img_dithered[i][j] < 128) ? 0 : 255;
            BYTE gay = img_dithered[i][j];
            // printf("gay: %d\n", gay);
            error = old - img_dithered[i][j];
            // printf("error: %d\n", error);
            if (i + 1 < height)
            {
                if (j + 1 < width) 
                {
                    img_dithered[i + 1][j + 1] = img_dithered[i + 1][j + 1] + (int)roundf((float)(error * 1)/16);
                    img_dithered[i    ][j + 1] = img_dithered[i    ][j + 1] + (int)roundf((float)(error * 7)/16); 
                }
                if(j - 1 >= 0)
                {
                    img_dithered[i + 1][j - 1] = img_dithered[i + 1][j - 1] + (int)roundf((float)(error * 3)/16); 
                }
                    img_dithered[i + 1][j    ] = img_dithered[i + 1][j    ] + (int)roundf((float)(error * 5)/16); 
            }
            if (i + 1 == height && j + 1 < width)
            {
                    img_dithered[i    ][j + 1] = img_dithered[i    ][j + 1] + (int)roundf((float)(error * 7)/16);
            }
            
                
                         
                
                
        }
            
    }

    int count = 0;
    for (int i = 0; i < width; i++)
    {
       printf("%X ",  img[0][i]);
       count ++;
    }
    printf("\nNumber of elements: %d\n", count);

    count = 0;
    printf("After dithering:\n");
    for (int i = 0; i < width; i++)
    {
       printf("%X ",  img_dithered[0][i]);
       count++;
    }
    printf("\nNumber of elements: %d\n", count);
}

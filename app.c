/**
 * 5. Write a program to convert the image file .BMP of 256 grey levels 
 * to binary image .BMP for the data section in the image file with the rule:
 *                     gray levels            binary      
 *                      0 – 127                    0
 *                      128 – 255                  1
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

void GreyScale(DWORD height, DWORD width, RGBTRIPLE img[height][width]);
void BinaryConvert(DWORD height, DWORD width, RGBTRIPLE img[height][width], RGBTRIPLE image_new[height][16]);
int main()
{
    FILE* fp, *cp;
    fp = fopen("Resources/barbara.bmp", "rb");         //bmp is binary file, therefore use "rb" permission
    cp = fopen("Resources/output.bmp", "wb");

    BITMAP_HEADER fHeader;
    INFO_HEADER   fInfo;

    //make an exact copy of target .bmp file
    // fseek(fp, 0, SEEK_SET);
    // char b;
    // for (int i = 0; i < fHeader.fSize; i++)
    // {
    //     fread(&b, 1, 1, fp);
    //     fwrite(&b, 1, 1, cp);
    // }
    // printf("Width: %d\n", fInfo.Width);
    // printf("Height: %d\n", fInfo.Height);
    // printf("fp after copying files: %ld\n", ftell(fp));
    
    //read file headers
    fseek(fp, 0, SEEK_SET);
    fread(&fHeader, sizeof(BITMAP_HEADER), 1, fp);
    fread(&fInfo, sizeof(INFO_HEADER), 1, fp);

    printf("Height: %d\n", fInfo.Height);
    printf("Width: %d\n", fInfo.Width);
    int width = fInfo.Width;
    int height = fInfo.Height;
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_width;

    //read pixel data
    fseek(fp, fHeader.fOffset, SEEK_SET);
    printf("before reading pixel data: %lu\n", ftell(fp));
    RGBTRIPLE (*image)[width] = (RGBTRIPLE *)malloc(height * sizeof(*image));
    RGBTRIPLE (*image_new)[width/8] = (RGBTRIPLE *)calloc(height, sizeof(*image));

    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(RGBTRIPLE), width, fp);
        fseek(fp, padding,  SEEK_CUR);      //skip over padding
    }
    int count = 0;
    
    for(int i = 0; i < width; i++)
    {
        printf("%X ", image[0][i].rgbtBlue);
        count++;
    }
    printf("\nNumber of elements: %d\n", count);

    BinaryConvert(height, width, image, image_new);

    int widthStep = fInfo.Width / 8;

    BYTE colorTable[8] = {0, 0, 0, 0, 255, 255, 255, 0};
    // BYTE colorTable[8] = {255,255,255,0,0,0,0,0};          //Endianness of BMP files is Little-Endian,
                                                                //thus the inverse arrangement
    // write file header
    fHeader.Signature = 0x4D42;
    fHeader.fOffset = 62;
    fHeader.fSize = widthStep * fInfo.Height + 62;
    fInfo.BitsPerPixel = 1;
    fInfo.imgSize = widthStep * fInfo.Height;
    fInfo.Color = 2;

    fseek(cp, 0, SEEK_SET);
    printf("cp after seek: %lu", ftell(cp));
    fwrite(&fHeader, sizeof(BITMAP_HEADER), 1, cp);
    fwrite(&fInfo, sizeof(INFO_HEADER), 1, cp);

    // printf("\ncp after reading bHeader: %lu\n", ftell(cp));
    fwrite(colorTable, sizeof(BYTE), 8, cp);
    // printf("cp after writing color palette: %lu\n", ftell(cp));

    //write pixel data to output
    fseek(cp, fHeader.fOffset, SEEK_SET);
    printf("cp before writing: %lu\n", ftell(cp));
    for (int i = 0; i < height; i++)
    {
        fwrite(image_new[i], sizeof(RGBTRIPLE), width/8, cp);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, cp);
        }
    }

    count = 0;
    printf("Pixel array after conversion: \n");
    for (int i = 0; i < width; i++)
    {
        printf("%X ", image[0][i].rgbtBlue);
        count++;
    }
    printf("\nNumber of elements: %d\n", count);

    count = 0;
    for (int i = 0; i < width/8; i++)
    {
        printf("%X ", image_new[0][i].rgbtBlue);
        count++;
    }
    printf("\nNew array: %d\n", count);

    fclose(cp);
    fclose(fp);   
}

void BinaryConvert(DWORD height, DWORD width, RGBTRIPLE img[height][width], RGBTRIPLE image_new[height][width/8])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (img[i][j].rgbtBlue <= 127 && img[i][j].rgbtBlue >= 0)
                img[i][j].rgbtBlue = 0;
            else if (img[i][j].rgbtBlue <= 255 && img[i][j].rgbtBlue >= 128)
                img[i][j].rgbtBlue = 0x1;
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        int k = 0;
        int count = 0;
        int shift = 7;
        int j = 0;
        for (j = 0; j < width/8; j++)
        {
            for (k = count; k < count + 8; k++)
            {   
                image_new[i][j].rgbtBlue += img[i][k].rgbtBlue << (shift - k);
                count = k;
                if (count % 512 == shift)
                {
                    count++;
                    shift = shift + 8;
                    break;
                }
            }
        }
    }
}


// void GreyScale(DWORD height, DWORD width, RGBTRIPLE img[height][width])
// {
//     float Grey;
//     for (int i = 0; i < height; i++)
//     {
//         for (int j = 0; j < width; j++)
//         {
//             Grey = (float)(img[i][j].rgbtBlue + img[i][j].rgbtGreen + img[i][j].rgbtRed)/3;
//             img[i][j].rgbtGreen = Grey;
//             img[i][j].rgbtBlue = Grey;
//             img[i][j].rgbtRed  = Grey;
//         }
//     }
// }
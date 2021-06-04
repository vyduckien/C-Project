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
#include <string.h>
#include <math.h>
#include <conio.h>
#include "bmp.h"
#include "functions.h"

int main()
{   FILE *src, *dst;
    char name[100];
    WelcomeMsg();

OPEN_FILE:
    printf("\nEnter path to image: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;       // remove newline from string
    src = fopen(name, "rb");            //bmp is binary file, therefore use "rb" permission
    dst = fopen("Resources/output.bmp", "wb");

    //check for valid file location
    if (src == NULL)
        {
            printf("File not found. Please enter a different path.");
            goto OPEN_FILE;
        }

    BITMAP_HEADER fHeader;
    INFO_HEADER   fInfo;
        
    //read header information
    // fseek(src, 0, SEEK_SET);
    // fread(&fHeader, sizeof(BITMAP_HEADER), 1, src);
    // fread(&fInfo, sizeof(INFO_HEADER), 1, src);
    ReadFile(src, &fHeader, &fInfo);

    DWORD width = (fInfo.Width % 2 == 1)? (fInfo.Width + 1) : fInfo.Width;  //width must be an even value
    DWORD height = fInfo.Height;
    DWORD padding = (4 - (width * sizeof(GRAY_VALUE)) % 4) % 4;             //calculate row paddings
    DWORD new_width = width/8;
    // printf("Height: %d\n", height);
    // printf("Width: %d\n", width);
    // printf("New Width: %d\n", new_width);

    //check for valid information
    if (fHeader.Signature != 0x4D42 || fInfo.BitsPerPixel != 8) 
    {
        printf("\nInvalid file. Please enter a different file.");
        goto OPEN_FILE;
    }
    
    //read pixel data
    fseek(src, fHeader.fOffset, SEEK_SET);
    GRAY_VALUE (*image)[width] = /* (GRAY_VALUE *) */malloc(height * sizeof(*image));
    GRAY_VALUE (*image_new)[new_width] = /* (GRAY_VALUE *) */calloc(height, sizeof(*image));    //allocate memory and initialize all with 0
    GRAY_VALUE (*image_dithered)[width] = calloc(height, sizeof(*image));
    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(GRAY_VALUE), width, src);
        fseek(src, padding,  SEEK_CUR);                     //skip over padding
    }

    //Convert grey levels to binary
    Dither(height, width, image, image_dithered);
    BinaryConvert(height, width, new_width, image_dithered, image_new);

    //Add color table for binary BMP file
    BYTE colorTable[8] = {0, 0, 0, 0, 255, 255, 255, 0};        //blue, green, red value. 1 will be white, 0 will be black
    //BYTE colorTable[8] = {255, 255, 255, 0, 0, 0, 0, 0};      //1 will be white, 0 will be black      

    //Modify header information
    NewHeader(&fHeader, &fInfo, new_width);
    
    //Write file header
    WriteFile(&dst, fHeader, fInfo, colorTable);

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
    int count = 0;
    for (int i = 0; i < width; i++)
    {
       printf("%d ",  image_dithered[0][i].Val);
       count++;
    }
    printf("\nNumber of elements: %d\n", count);
    //Complete and close all files
    fclose(dst);
    fclose(src);
    free(image);
    free(image_new);
    int input;
    printf("Process complete.\n");
END_PROGRAM:
    printf("Press 1 to exit, or 0 to restart the program.");
    scanf("%d", &input);
    getchar();      //consume newline for next input
    switch(input)
    {
        case 1:
            break;
        case 0:
            goto OPEN_FILE;
            break;
        default:
            printf("Invalid input.\n");
            goto END_PROGRAM;
            break;
    }
}
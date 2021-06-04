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
    int input;
    char name[100];
    char save[100];
    WelcomeMsg();

OPEN_FILE:
    printf("\nEnter path to image: ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;       // remove newline from string
    printf("\nPlease choose where the result will be stored: ");
    fgets(save, 100, stdin);
    save[strcspn(save, "\n")] = 0;       // remove newline from string
    src = fopen(name, "rb");            //bmp is binary file, therefore use "rb" permission
    dst = fopen(save, "wb");

    //check for valid file location
    if (src == NULL)
        {
            printf("File not found. Please enter a different path.");
            goto OPEN_FILE;
        }

    BITMAP_HEADER fHeader;
    INFO_HEADER   fInfo;
    
    // fread(&fInfo, sizeof(INFO_HEADER), 1, src);
    ReadFile(src, &fHeader, &fInfo);
    
    DWORD width = (fInfo.Width % 2 == 1)? (fInfo.Width + 1) : fInfo.Width;  //width must be an even value
    DWORD height = fInfo.Height;
    DWORD padding = (4 - (width * sizeof(BYTE)) % 4) % 4;             //calculate row paddings
    DWORD new_width = width/8;

    //check for valid information
    if (fHeader.Signature != 0x4D42 || fInfo.BitsPerPixel != 8) 
    {
        printf("\nInvalid file. Expected BMP format.\nPlease enter a different file.");
        goto OPEN_FILE;
    }
    
    //read pixel data
    fseek(src, fHeader.fOffset, SEEK_SET);
    BYTE (*image)[width] = /* (GRAY_VALUE *) */malloc(height * sizeof(*image));
    BYTE (*image_new)[new_width] = /* (GRAY_VALUE *) */calloc(height, sizeof(*image));    //allocate memory and initialize all with 0
    BYTE (*image_dithered)[width] = calloc(height, sizeof(*image));
    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(BYTE), width, src);
        fseek(src, padding,  SEEK_CUR);                     //skip over padding
    }
    
    //Convert grey levels to binary
    printf("Choose an options:\n");
    printf("\t1 - With dithering\n");
    printf("\t0 - No dithering\n");
    printf("Your option? (0 - 1): ");        //Let user choose between dithering or non-dithering
    scanf("%d", &input);
    getchar();                              //consume newline for future input
    switch(input)
    {
        case 1:
            Dither(height, width, image, image_dithered);
            BinaryConvert(height, width, new_width, image_dithered, image_new);
            break;
        case 0:
            BinaryConvert(height, width, new_width, image, image_new);
            break;
    }

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
        fwrite(image_new[i], sizeof(BYTE), new_width, dst);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, dst);
        }
    }

    //Complete and close all files
    fclose(dst);
    fclose(src);
    free(image);
    free(image_new);
    printf("Process complete.\n");
    printf("Resultant image is in ");
    printf("%s\n", save);
END_PROGRAM:
    printf("Press 1 to exit, or 0 to restart the program.");
    scanf("%d", &input);
    getchar();      //consume newline for future input
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
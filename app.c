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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include "bmp.h"
#include "functions.h"

int main()
{   FILE *src, *dst;
    BITMAP_HEADER fHeader;
    INFO_HEADER   fInfo;
    int input;
    char name[100];
    char save[100];
    WelcomeMsg();

OPEN_FILE:
    //get file directory and save location from user
    printf("\nEnter path to image (C:/Program Files/input.bmp): ");
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = 0;          //remove newline from string
    src = fopen(name, "rb");                //bmp is a binary file, therefore use "rb" permission

    //check for valid file location
    if (src == NULL)
        {
            printf("File not found. Please enter a different path.");
            goto OPEN_FILE;
        }

    // Read file header
    ReadFile(src, &fHeader, &fInfo);

    //check for valid information
    if (fHeader.Signature != 0x4D42 || fInfo.BitsPerPixel != 8) 
    {
        printf("\nInvalid file. Expected BMP format.\nPlease choose a different file.");
        fclose(src);
        goto OPEN_FILE;
    }

    char *base = basename(name);
    printf("\nImage information: \n");
    printf("\tName: %s\n", base);
    printf("\tSize: %0.1f KiB\n", (float)fHeader.Size/1024);
    printf("\tWidth: %d px\n", fInfo.Width);
    printf("\tHeight: %d px\n", fInfo.Height);
    printf("\tColor depth (in bits): %d\n", fInfo.BitsPerPixel);

    //open file for writing the result
    printf("\nChoose where the result will be saved (Ex: C:/Program Files/output.bmp): ");
    fgets(save, 100, stdin);
    save[strcspn(save, "\n")] = 0;          //remove newline from string
    dst = fopen(save, "wb");    
    
    DWORD width = (fInfo.Width % 2 == 1)? (fInfo.Width + 1) : fInfo.Width;  //width must be an even value
    DWORD height = fInfo.Height;
    DWORD padding = (4 - (width * sizeof(BYTE)) % 4) % 4;                   //calculate row paddings
    DWORD new_width = width/8;
    
    //read pixel data
    fseek(src, fHeader.Offset, SEEK_SET);
    BYTE (*image)[width] = malloc(height * sizeof(*image));
    BYTE (*image_new)[new_width]  = calloc(height, sizeof(*image));          //allocate memory and initialize all with 0
    BYTE (*image_dithered)[width] = calloc(height, sizeof(*image));
    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(BYTE), width, src);
        fseek(src, padding,  SEEK_CUR);                     //skip over padding
    }
    // int count = 0;
    // for (int i = 0; i < width; i++)
    // {
    //    printf("%d ", image[234][i]);
    //    count++;
    //    if (count % 8 == 0) printf("\n");
    // }
    // printf("\nNumber of elements: %d\n", count);
    
    //Convert grey levels to binary
    DitherOpt();                            //Let user choose between dithering or non-dithering
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
            // count = 0;
            // for (int i = 0; i < new_width; i++)
            // {
            //     printf("%d. %X\n", count, image_new[234][i]);
            //     count++;
            // }
            //     printf("\nNew number of elements: %d\n", count);
            break;
    }

    //Add color table for binary BMP file
    BYTE colorTable[8] = {0, 0, 0, 0, 255, 255, 255, 0};        //blue, green, red value. 1 will be white, 0 will be black
    // BYTE colorTable[8] = {255, 255, 255, 0, 0, 0, 0, 0};      //1 will be white, 0 will be black      

    //Modify header information
    NewHeader(&fHeader, &fInfo, new_width);
    
    //Write file header
    WriteHeader(&dst, fHeader, fInfo, colorTable);

    //write pixel data to output
    fseek(dst, fHeader.Offset, SEEK_SET);
    WriteData(height, new_width, dst, image_new, padding);

    //Complete and close all files
    fclose(dst);
    fclose(src);
    free(image);
    free(image_new);
    base = basename(save);
    printf("\nProcessing...");
    printf("\nProcess complete.\n");
    printf("\nResult information: \n");
    printf("\tName: %s\n", base);
    printf("\tSize: %.1f KiB\n", (float)fHeader.Size/1024);
    printf("\tWidth: %d px\n", width);
    printf("\tHeight: %d px\n", fInfo.Height);
    printf("\tColor depth (in bits): %d\n", fInfo.BitsPerPixel);
    printf("Go to output.bmp to see the result.\n");

END_PROGRAM:
    printf("\nPress 1 to exit, or 0 to restart the program.");
    printf("\nYour option? (0 - 1) ");
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
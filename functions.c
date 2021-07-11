#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "bmp.h"
#include "functions.h"

void WelcomeMsg()
{   
    printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf(
      "       ___       __   __         ___    ___  __     ___       ___     __      __   __   __        ___  __  ___\n" 
    " |  | |__  |    /  ` /  \\  |\\/| |__      |  /  \\     |  |__| |__     /  `    |__) |__) /  \\    | |__  /  `  |\n"
   " |/\\| |___ |___ \\__, \\__/  |  | |___     |  \\__/     |  |  | |___    \\__,    |    |  \\ \\__/ \\__/ |___ \\__,  |\n\n");
    printf("+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
    printf("\n\nPress Ctrl + C to exit the program at any point.");
    ASK_INPUT:
    printf("\n\nPress 0 to begin the program.");
    printf("\n      1 to exit the program.");
    printf("\n      2 for detailed instructions. (Recommended)");    
    printf("\nYour option? (0 - 2) ");
    int input;
    scanf("%d", &input);
    if ((int)input < 0 || (int)input > 2)
    {
        printf("Try again.");
        delay(500);
        getchar();
        goto ASK_INPUT;
    }
    getchar();
    while(1)
    {
        switch(input)
        {
            case 0:
                return;
                break;
            case 1:
                printf("\nExiting the program...");
                delay(1000);
                exit(EXIT_FAILURE);
                break;
            case 2:
                printf("\n HOW TO USE THE PROGRAM:");
                printf("\n\t1. Enter the path to the image. Note: image must be a .BMP file.\n"
                        "\t\tEx: C:\\Program Files\\input.bmp"
                        "\n\t2. Enter the location where the result will be saved."
                        "\n\t\tEx: C:\\Program Files\\output.bmp"
                        "\n\tNote: If the program's executable file and the image file are in the same folder,"
                        "\n\tjust enter the name of the image file and proceed."
                        "\n\t\tEx: abcd.bmp");
                printf("\n\t3. Select Dithering mode (with or without Dithering).");
                printf("\n\t4. Wait until the process is complete and see the result at the given location.");
                goto ASK_INPUT;
                getchar();
                break; 
            default:
                goto ASK_INPUT;
                break;
        }
    }
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
    (*fHeader).Offset = 62;
    (*fHeader).Size = new_width * (*fInfo).Height + 62;
    (*fInfo).BitsPerPixel = 1;
    (*fInfo).imgSize = new_width * (*fInfo).Height;
    (*fInfo).Color = 2;
}

void WriteHeader(FILE **dst, BITMAP_HEADER fHeader, INFO_HEADER fInfo, BYTE colorTable[])
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
            img_dithered[i][j] = (img_dithered[i][j] < 128) ? 0 : 255;
            error = old - img_dithered[i][j];
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
}

void WriteData(DWORD height, DWORD new_width, FILE* dst, BYTE img_new[][new_width], DWORD padding)
{
    for (int i = 0; i < height; i++)
    {
        fwrite(img_new[i], sizeof(BYTE), new_width, dst);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, dst);                       //insert padding
        }
    }
}

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
    
    //squeeze 8 bytes of 0 and 1 from the original image into one byte using bit shifting
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

char* getFileNameFromPath(char* path)
{
    if( path == NULL )
        return NULL;

    char *pFileName = path;
    for( char * pCur = path; *pCur != '\0'; pCur++)
    {
        if( *pCur == '/' || *pCur == '\\' )
            pFileName = pCur + 1;
    }
    return pFileName;
}

void delay(int milli_secs)
{ 
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_secs);
}
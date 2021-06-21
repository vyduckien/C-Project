#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "bmp.h"
#include "functions.h"



void WelcomeMsg()
{   
    printf("=========================================================================================================================================================================\n");
    printf(
        "__          __ ______  _       _____  ____   __  __  ______   _______  ____    _______  _    _  ______    _____   _____   _____    ____        _  ______  _____  _______\n" 
        "\\ \\        / /|  ____|| |     / ____|/ __ \\ |  \\/  ||  ____| |__   __|/ __ \\  |__   __|| |  | ||  ____|  / ____| |  __ \\ |  __ \\  / __ \\      | ||  ____|/ ____||__   __|\n"
        " \\ \\  /\\  / / | |__   | |    | |    | |  | || \\  / || |__       | |  | |  | |    | |   | |__| || |__    | |      | |__) || |__) || |  | |     | || |__  | |        | |\n"  
        "  \\ \\/  \\/ /  |  __|  | |    | |    | |  | || |\\/| ||  __|      | |  | |  | |    | |   |  __  ||  __|   | |      |  ___/ |  _  / | |  | | _   | ||  __| | |        | |\n"  
        "   \\  /\\  /   | |____ | |____| |____| |__| || |  | || |____     | |  | |__| |    | |   | |  | || |____  | |____  | |     | | \\ \\ | |__| || |__| || |____| |____    | |\n"   
        "    \\/  \\/    |______||______|\\_____|\\____/ |_|  |_||______|    |_|   \\____/     |_|   |_|  |_||______|  \\_____| |_|     |_|  \\_\\ \\____/  \\____/ |______|\\_____|   |_|\n");
    printf("=========================================================================================================================================================================\n");
    printf("\n\nPress Ctrl + C to exit the program at any point.");
    printf("\n\nPress 0 to begin the program.");
    printf("\n      1 to exit the program.");
    printf("\n      2 for detailed instructions. (Recommended)");
    printf("\nYour option? (0 - 2) ");
    int input;
    scanf("%d", &input);
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
                exit(EXIT_FAILURE);
                break;
            case 2:
                printf("\n HOW TO USE THE PROGRAM:");
                printf("\n\t1. Enter the path to the image. Note: image make be a .BMP file.\n"
                        "\t\tEx: C:/Program Files/input.bmp"
                        "\n\t2. Enter the location where the result will be saved."
                        "\n\t\tEx: C:/Program Files/output.bmp"
                        "\n\tNote: If the program's executable file and the image file are in the same folder,"
                        "\n\tjust enter the name of the image file and proceed."
                        "\n\t\tEx: abcd.bmp");
                printf("\n\t3. Select Dithering mode (with or without Dithering).");
                printf("\n\t4. Wait until the process is complete and see the result at the given location.");
                printf("\n\n\tPress 0 to begin the program, or 1 to exit. ");
                printf("\n\tYour option? (0 - 1) ");
                scanf("%d", &input);
                getchar();
                break; 
            default:
                input = 1;
                break;
        }
    }
}

void DitherOpt()
{
    printf("\nChoose an option:\n");
    printf("\t1 - With dithering\n");
    printf("\t0 - No dithering\n");
    printf("Your option? (0 - 1): ");
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
    // int count = 0;
    // for (int i = 0; i < width; i++)
    // {
    //    printf("%X ",  img[0][i]);
    //    count ++;
    // }
    // printf("\nNumber of elements: %d\n", count);

    // count = 0;
    // printf("After dithering:\n");
    // for (int i = 0; i < width; i++)
    // {
    //    printf("%X ",  img_dithered[0][i]);
    //    count++;
    // }
    // printf("\nNumber of elements: %d\n", count);
}

void WriteData(DWORD height, DWORD new_width, FILE* dst, BYTE img_new[][new_width], DWORD padding)
{
    for (int i = 0; i < height; i++)
    {
        fwrite(img_new[i], sizeof(BYTE), new_width, dst);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, dst);
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

char *basename(char const *path)
{
        char *s = strrchr(path, '/');
        if(s==NULL) {
                return strdup(path);
        } else {
                return strdup(s + 1);
        }
}
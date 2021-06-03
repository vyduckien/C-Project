#include "bmp.h"

void BinaryConvert(DWORD height, DWORD width, DWORD new_width, RGBTRIPLE img[height][width], RGBTRIPLE image_new[height][new_width])
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
        for (j = 0; j < new_width; j++)
        {
            for (k = count; k < count + 8; k++)
            {   
                image_new[i][j].rgbtBlue += img[i][k].rgbtBlue << (shift - k);
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
# Welcome to the repository of the program

This is the program that converts an greyscale BMP file of 8-bit color depth to a binary bmp file.

This program does not simply change the color of the original image, but __*also modifies the header information*__.

The resultant image contain the new header file corresponding to the dimension and its color depth, as well as reduced size due to compression.

Due to the fact that in 1-bit color BMP file there is only one 1 bit for a pixel instead of 8 bits in the case of an 8-bit BMP file, the file size should theoretically be reduced by **1/8**.

Reference resources:

BMP file format: https://en.wikipedia.org/wiki/BMP_file_format

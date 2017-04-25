/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }
    
    int n = atoi(argv[1]);
    if (n < 1 || n > 100)
    {
        printf("n must be a positive number less than or equal to 100\n");
        return 5;
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    int oldWidth = bi.biWidth;
    int oldHeight = bi.biHeight;
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;
    
    int old_padding = (4 - (oldWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Writing new sizes to headers of the new resized file
    bi.biSizeImage = (bi.biWidth * abs(bi.biHeight) * sizeof(RGBTRIPLE)) + (padding * abs(bi.biHeight));
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(oldHeight); i < biHeight; i++)
    {
        int offset = oldWidth * sizeof(RGBTRIPLE);
        
        for (int counter = 0; counter < n; counter++) 
        {
            // iterate over pixels in scanline
            for (int j = 0; j < oldWidth; j++)
            {
    
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                    // write RGB triple to outfile
                    for (int dbl = n; dbl > 0; dbl --)
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        
            fseek(inptr, -offset, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    
        fseek(inptr, old_padding + offset, SEEK_CUR);
    }
    

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j].rgbtRed == 0 && image[i][j].rgbtGreen == 0 && image[i][j].rgbtBlue == 0)
            {
                break;
            }
            
            int avrg_col = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3;
            image[i][j].rgbtRed = avrg_col;
            image[i][j].rgbtGreen = avrg_col;
            image[i][j].rgbtBlue = avrg_col;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int addRed = 0;
            int addGreen = 0;
            int addBlue = 0;
            // account for edges, since there are no pixels over/behind/below/past them
            int koffset = 0;
            int loffset = 0;
            int kend = 0;
            int lend = 0;

            if (i == 0) // top edge
            {
                koffset = 1;
            }else if (i == height - 1) // bottom edge
            {
                kend = -1;
            }

            if (j == 0) // left edge
            {
                loffset = 1;
            }else if (j == width - 1) // right edge
            {
                lend = -1;
            }
            
            // set pixel's RGB values to average of RGB of 3x3 grid of pixels around them
            int count = 0;
            for (int k = -1 + koffset; k < 2 + kend; k++)
            {
                for (int l = -1 + loffset; l < 2 + lend; l++)
                {
                    addRed += image[i + k][j + l].rgbtRed;
                    addGreen += image[i + k][j + l].rgbtGreen;
                    addBlue += image[i + k][j + l].rgbtBlue;
                    count++;
                }
            }
            image[i][j].rgbtRed = addRed / count;
            image[i][j].rgbtGreen = addGreen / count;
            image[i][j].rgbtBlue = addBlue / count;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tempArr[height][width];

    int kernelGy[3][3] = {{-1,-2,-1},
                          { 0, 0, 0},
                          { 1, 2, 1}};

    int kernelGx[3][3] = {{-1, 0, 1},
                          {-2, 0, 2},
                          {-1, 0, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // account for image edges, since there are no pixels over/behind/below/past them
            int kstart = 0;
            int lstart = 0;
            int kend = 0;
            int lend = 0;

            if (i == 0) // top edge
            {
                kstart = 1;
            }else if (i == height - 1) // bottom edge
            {
                kend = -1;
            }

            if (j == 0) // left edge
            {
                lstart = 1;
            }else if (j == width - 1) // right edge
            {
                lend = -1;
            }
            
            // for each pixel, add weighted RGB values of pixels around it according to kernels
            int redGx = 0, greenGx = 0, blueGx = 0;
            int redGy = 0, greenGy = 0, blueGy = 0;
            float redG, greenG, blueG;
            for (int k = -1 + kstart; k < 2 + kend; k++)
            {
                for (int l = -1 + lstart; l < 2 + lend; l++)
                {
                    // for gy
                    redGy += image[i + k][j + l].rgbtRed * kernelGy[k + 1][l + 1];
                    greenGy += image[i + k][j + l].rgbtGreen * kernelGy[k + 1][l + 1];
                    blueGy += image[i + k][j + l].rgbtBlue * kernelGy[k + 1][l + 1];
                    // fox gx
                    redGx += image[i + k][j + l].rgbtRed * kernelGx[k + 1][l + 1];
                    greenGx += image[i + k][j + l].rgbtGreen * kernelGx[k + 1][l + 1];
                    blueGx += image[i + k][j + l].rgbtBlue * kernelGx[k + 1][l + 1];
                }
            }

            // sobel algo
            redG = sqrt((redGx * redGx) + (redGy * redGy));
            greenG = sqrt((greenGx * greenGx) + (greenGy * greenGy));
            blueG = sqrt((blueGx * blueGx) + (blueGy * blueGy));

            // cap value to 255
            if (redG > 255)
            {
                redG = 255;
            }
            if (greenG > 255)
            {
                greenG = 255;
            }
            if (blueG > 255)
            {
                blueG = 255;
            }

            // create new image in temp array
            tempArr[i][j].rgbtRed = (int)redG;
            tempArr[i][j].rgbtGreen = (int)greenG;
            tempArr[i][j].rgbtBlue = (int)blueG;
        }
    }

    // pass new image into original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = tempArr[i][j];
        }   
    }    
}

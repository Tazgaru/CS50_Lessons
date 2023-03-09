#include "helpers.h"

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
    return;
}

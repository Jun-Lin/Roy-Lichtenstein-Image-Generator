#include <iostream>
#include <cmath>
#include <cstdlib>
#include "turtle.h"
#include <vector>
#include <map>
#include <algorithm>

extern "C" {
#include "dib.h"
#include "draw.h"
#include "dib_Fonts.h"
}

// Am I on a mac
#define MACOSX

float sqrDistance(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2)
{
  float a = _x1 - _x2;
  float b = _y1 - _y2;
  float c = _z1 - _z2;

  return ( (a*a) + (b*b) + (c*c) );
}

int blur_filter(const char* infilename, const char* outfilename, int radius)
{
    pdib image_0 = dib_Load(infilename);

    int depth = dib_Bitcount(image_0);
    int width  = dib_Width(image_0);
    int height = dib_Height(image_0);
    int storage_width = dib_StorageWidth(image_0);
    unsigned char* buffer = dib_Image(image_0);
    int byte_count = depth/8;

    for (int y=radius; y<=height-radius-1; y++)
    {
        int scanline1 = y * storage_width;

        for (int x=radius; x<=width-radius; x++)
        {
            int offset1 = x * byte_count;

            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            for (int i=-radius; i<=radius; i++)
            {
                for (int j=-radius; j<=radius; j++)
                {
                    if ( (y+i) > height || (x+j) > width) break;

                    int scanline = (y+i) * storage_width;
                    int offset = (x+j) * byte_count;

                    sumR = sumR + buffer[scanline+offset+0];
                    sumG = sumG + buffer[scanline+offset+1];
                    sumB = sumB + buffer[scanline+offset+2];
                }
            }

            int averageR = sumR / pow((radius*2)+1,2);
            int averageG = sumG / pow((radius*2)+1,2);
            int averageB = sumB / pow((radius*2)+1,2);

            buffer[scanline1+offset1+0] = averageR;
            buffer[scanline1+offset1+1] = averageG;
            buffer[scanline1+offset1+2] = averageB;
        }
    }

    //Update Image:
    dib_Write(image_0, outfilename);
    //Clean up:
    dib_Destroy(image_0);
    return 0;
}

int reduce_colour(const char* infilename, const char* outfilename,int level)
{
    pdib image_0 = dib_Load(infilename);

    int depth = dib_Bitcount(image_0);
    int width  = dib_Width(image_0);
    int height = dib_Height(image_0);
    int storage_width = dib_StorageWidth(image_0);
    unsigned char* buffer = dib_Image(image_0);
    int byte_count = depth/8;

    float div = 255.0/level;

    for (int y=0; y<height; y++)
    {
        int scanline = y * storage_width;

        for (int x=0; x<width; x++)
        {
            int offset = x * byte_count;

            float count = -div;
            float count2 = 0;

            int red = buffer[scanline+offset+0];
            int green = buffer[scanline+offset+1];
            int blue = buffer[scanline+offset+2];

            for (int i=0; i<=level-1; i++)
            {
                count += div;
                count2 += div;

                float balance = (count + count2)/2;
                balance = round(balance);

                if (red >= count && red < count2)
                {
                    buffer[scanline+offset+0] = balance;
                }

                if (green >= count && green < count2)
                {
                    buffer[scanline+offset+1] = balance;
                }

                if (blue >= count && blue < count2)
                {
                    buffer[scanline+offset+2] = balance;
                }
            }

        }
    }

    //Update Image:
    dib_Write(image_0, outfilename);
    //Clean up:
    dib_Destroy(image_0);
    return 0;
}

int posterize_filter(const char* infilename, const char* outfilename)
{
    //Loads in the image:
    pdib image_0 = dib_Load(infilename);

    int depth = dib_Bitcount(image_0);
    int width  = dib_Width(image_0);
    int height = dib_Height(image_0);
    int storage_width = dib_StorageWidth(image_0);
    unsigned char* buffer = dib_Image(image_0);
    int byte_count = depth/8;

    //Roy Lichtenstein Main Colour Palette
    int colourR[] = {192,33,60};
    int colourY[] = {247,197,72};
    int colourB[] = {24,79,133};
    int colourW[] = {255,255,255};

    for (int y=0; y < height; y++)
    {
        int scanline = y * storage_width;

        for (int x=0; x < width; x++)
        {
            int offset = x * byte_count;

            int redValue = buffer[scanline+offset+0];
            int greenValue = buffer[scanline+offset+1];
            int blueValue = buffer[scanline+offset+2];

            //Euclidean Distance
            float redDistance = sqrDistance(redValue, greenValue, blueValue, colourR[0], colourR[1], colourR[2]);
            float yellowDistance = sqrDistance(redValue, greenValue, blueValue, colourY[0], colourY[1], colourY[2]);
            float blueDistance = sqrDistance(redValue, greenValue, blueValue, colourB[0], colourB[1], colourB[2]);
            float whiteDistance = sqrDistance(redValue, greenValue, blueValue, colourW[0], colourW[1], colourW[2]);

            float minDistance = fmin(redDistance, fmin(yellowDistance, fmin(blueDistance, whiteDistance)));

            if (minDistance==redDistance)
            {
                buffer[scanline+offset+0] = colourR[0];
                buffer[scanline+offset+1] = colourR[1];
                buffer[scanline+offset+2] = colourR[2];
            }

            else if (minDistance==yellowDistance)
            {
                buffer[scanline+offset+0] = colourY[0];
                buffer[scanline+offset+1] = colourY[1];
                buffer[scanline+offset+2] = colourY[2];
            }

            else if (minDistance==blueDistance)
            {
                buffer[scanline+offset+0] = colourB[0];
                buffer[scanline+offset+1] = colourB[1];
                buffer[scanline+offset+2] = colourB[2];
            }

            else if (minDistance==whiteDistance)
            {
                buffer[scanline+offset+0] = colourW[0];
                buffer[scanline+offset+1] = colourW[1];
                buffer[scanline+offset+2] = colourW[2];
            }

        }
    }

    //Update Image:
    dib_Write(image_0, outfilename);
    //Clean up:
    dib_Destroy(image_0);
    return 0;
}

int edge_filter(const char* infilename, const char* outfilename, int edgeStrength)
{
    pdib image_0 = dib_Load(infilename);

    int depth = dib_Bitcount(image_0);
    int width  = dib_Width(image_0);
    int height = dib_Height(image_0);
    int storage_width = dib_StorageWidth(image_0);
    unsigned char* buffer = dib_Image(image_0);
    int byte_count = depth/8;
    int GX[3][3], GY[3][3];
    int SUM;
    std::vector<int> SUMlist;

    //Sobel Matrices Horizontal
    GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
    GX[1][0] = -edgeStrength; GX[1][1] = 0; GX[1][2] = edgeStrength;
    GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;

    //Sobel Matrices Vertical
    GY[0][0] = 1; GY[0][1] = edgeStrength; GY[0][2] = 1;
    GY[1][0] = 0; GY[1][1] = 0; GY[1][2] = 0;
    GY[2][0] = -1; GY[2][1] = -edgeStrength; GY[2][2] = -1;

    for (int y=0; y<height; y++)
    {
        for (int x=0; x<width; x++)
        {
            int sumX = 0;
            int sumY = 0;

            if (y==0 || y==height-1) SUM=0;
            else if (x==0 || x==width-1) SUM=0;

            else
            {
                for (int i=-1; i<=1; i++)
                {
                    for (int j=-1; j<=1; j++)
                    {
                        int scanline = (y+i) * storage_width;
                        int offset = (x+j) * byte_count;

                        int colour = buffer[scanline+offset];

                        sumX = sumX + (colour * GX[i+1][j+1]);
                        sumY = sumY + (colour * GY[i+1][j+1]);
                    }
                }

                SUM = sqrt(pow((double)sumX,2) + pow((double)sumY,2));

            }

            if (SUM>255) SUM=255;
            if (SUM<0) SUM=0;

            SUMlist.push_back(SUM);
            SUMlist.push_back(SUM);
            SUMlist.push_back(SUM);

        }
    }

    int size = SUMlist.size();
    for (int i=0; i<=size; i++)
    {
        buffer[i] = 255-SUMlist[i];
    }

    dib_Write(image_0, outfilename);

    // clean up
    dib_Destroy(image_0);

    return 0;
}

int simplify_filter(const char* infilename, const char* overlay, const char* outfilename, int detail)
{
    pdib image_0 = dib_Load(infilename);

    int depth = dib_Bitcount(image_0);
    int width  = dib_Width(image_0);
    int height = dib_Height(image_0);
    int storage_width = dib_StorageWidth(image_0);
    unsigned char* buffer = dib_Image(image_0);
    int byte_count = depth/8;
    std::vector<int> colourListX;
    std::vector<int> colourListY;

    for (int y=0; y < height; y++)
    {
        int scanline = y * storage_width;

        for (int x=0; x < width; x++)
        {
            int offset = x * byte_count;

            int colour = buffer[scanline+offset];

            if (colour<=detail)
            {
                colourListX.push_back(x);
                colourListY.push_back(y);
            }

            buffer[scanline+offset+0] = 255;
            buffer[scanline+offset+1] = 255;
            buffer[scanline+offset+2] = 255;
        }
    }

    pdib image_1 = dib_Load(overlay);

    int colour0[] = {0, 0, 0, 255};

    int size = colourListX.size();
    for (int i=0; i<size; i++)
    {
        ImagingDrawPoint(image_1, colourListX[i], colourListY[i], colour0, 1);
    }


    dib_Write(image_1, outfilename);
    // clean up
    dib_Destroy(image_1);

    return 0;
}

int main()
{
    pdib image_0 = dib_Load("test_face.bmp");

    blur_filter("test_face.bmp", "Roy_Lichtenstein_Image.bmp", 1);
    reduce_colour("Roy_Lichtenstein_Image.bmp", "Roy_Lichtenstein_Image.bmp", 20);
    posterize_filter("Roy_Lichtenstein_Image.bmp", "Roy_Lichtenstein_Image.bmp");

    blur_filter("test_face.bmp", "Roy_Lichtenstein_Image_Edge.bmp", 3);
    edge_filter("Roy_Lichtenstein_Image_Edge.bmp", "Roy_Lichtenstein_Image_Edge.bmp", 10);
    simplify_filter("Roy_Lichtenstein_Image_Edge.bmp", "Roy_Lichtenstein_Image.bmp", "Roy_Lichtenstein_Image.bmp", 100);

    //Clean up
    dib_Destroy(image_0);

    #ifdef MACOSX
    system("open \"Roy_Lichtenstein_Image.bmp\"&");
    #else
    system("display \"Roy_Lichtenstein_Image.bmp\"&");
    #endif

    return EXIT_SUCCESS;
}

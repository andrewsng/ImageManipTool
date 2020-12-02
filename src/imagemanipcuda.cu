#include "imagemanip.h"
#include "imagemanipcuda.h"
#include "mat2.h"
#include "vec2.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstddef>
using std::size_t;
#include <algorithm>
using std::max;
using std::min;
using std::swap;
#include <vector>
using std::vector;
using FilterType1D = vector<float>;
using FilterType2D = vector<vector<float>>;


__global__ void transformKernel(float * input, float * output, float minX, float minY, 
                                float dx, float dy, int sWidth, int sHeight, int dWidth, int dHeight, float * mat)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    float tx = mat[0] * (minX + x * dx) + mat[1] * (minY + y * dy);
    float ty = mat[2] * (minX + x * dx) + mat[3] * (minY + y * dy);

    if (0.0f <= tx && tx < float(sWidth) - 1.0f &&
        0.0f <= ty && ty < float(sHeight) - 1.0f)
    {
        output[x*3+0 + ((dHeight-1-y)*3 * dWidth)] = input[int(tx)*3+0 + ((sHeight-1-int(ty)) * 3 * sWidth)];
        output[x*3+1 + ((dHeight-1-y)*3 * dWidth)] = input[int(tx)*3+1 + ((sHeight-1-int(ty)) * 3 * sWidth)];
        output[x*3+2 + ((dHeight-1-y)*3 * dWidth)] = input[int(tx)*3+2 + ((sHeight-1-int(ty)) * 3 * sWidth)];
    }
}


// Image::Pixel bilinearInterp(const Image & image, float x, float y)
// {
//     int x0 = min(int(x), image.width() - 1);
//     int y0 = min(int(y), image.height() - 1);
//     int x1 = min(x0 + 1, image.width() - 1);
//     int y1 = min(y0 + 1, image.height() - 1);
//     Image::Pixel c00 = image.getColor(x0, y0);
//     Image::Pixel c01 = image.getColor(x0, y1);
//     Image::Pixel c10 = image.getColor(x1, y0);
//     Image::Pixel c11 = image.getColor(x1, y1);
//     float biasX = x - x0;
//     float biasY = y - y0;

//     Image::Pixel result;
//     result.r = (1.0f - biasY) * ((1.0f - biasX) * c00.r + biasX * c10.r) + biasY * ((1.0f - biasX) * c01.r + biasX * c11.r);
//     result.g = (1.0f - biasY) * ((1.0f - biasX) * c00.g + biasX * c10.g) + biasY * ((1.0f - biasX) * c01.g + biasX * c11.g);
//     result.b = (1.0f - biasY) * ((1.0f - biasX) * c00.b + biasX * c10.b) + biasY * ((1.0f - biasX) * c01.b + biasX * c11.b);

//     return result;
// }


Image transformCuda(const Image & image, Mat2 transform)
{
    float maxX = float(image.width() - 1);
    float maxY = float(image.height() - 1);
    Vec2 topLeft(0.0f, maxY);
    Vec2 topRight(maxX, maxY);
    Vec2 botLeft(0.0f, 0.0f);
    Vec2 botRight(maxX, 0.0f);

    topLeft = transform * topLeft;
    topRight = transform * topRight;
    botLeft = transform * botLeft;
    botRight = transform * botRight;

    float newMaxX = max({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMinX = min({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMaxY = max({ topLeft.y, topRight.y, botLeft.y, botRight.y });
    float newMinY = min({ topLeft.y, topRight.y, botLeft.y, botRight.y });

    Image transformed(newMaxX - newMinX + 1, newMaxY - newMinY + 1);

    float dx = (newMaxX - newMinX) / transformed.width();
    float dy = (newMaxY - newMinY) / transformed.height();

    Image source;
    // float sizeRatio = float(transformed.width() * transformed.height()) / (image.width() * image.height());
    // if (sizeRatio < 0.25)
    //     source = gaussianBlurSeparableCuda(image, 9, 3);
    // else if (sizeRatio > 1.5f)
    //     source = gaussianBlurSeparableCuda(image, 3, 1);
    // else
        source = image;

    Mat2 transformInv = transform.inverse();

    float * input;
    cudaMalloc(&input, source.width() * source.height() * sizeof(float) * 3);
    cudaMemcpy(input, &source(0, 0), source.width() * source.height() * sizeof(float) * 3, cudaMemcpyHostToDevice);
    float * output;
    cudaMalloc(&output, transformed.width() * transformed.height() * sizeof(float) * 3);
    float * mat;
    cudaMalloc(&mat, 4 * sizeof(float));
    cudaMemcpy(mat, &transformInv[0][0], 4 * sizeof(float), cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    dim3 gridSize((transformed.width()  + blockSize.x - 1) / blockSize.x,
                  (transformed.height() + blockSize.y - 1) / blockSize.y);

    transformKernel<<<gridSize, blockSize>>>(input, output, newMinX, newMinY, dx, dy,
        source.width(), source.height(), transformed.width(), transformed.height(), mat);

    cudaDeviceSynchronize();
    cudaMemcpy(&transformed(0, 0), output, transformed.width() * transformed.height() * sizeof(float) * 3, cudaMemcpyDeviceToHost);

    cudaFree(input);
    cudaFree(output);
    cudaFree(mat);

    return transformed;
}


Image rotateCuda(const Image & image, float angleDeg)
{
    float angleRad = M_PI * angleDeg / 180.0f;
    float cosAngle = cosf(angleRad);
    float sinAngle = sinf(angleRad);
    Mat2 rotMat = {
        { cosAngle, -sinAngle },
        { sinAngle,  cosAngle }
    };

    return transformCuda(image, rotMat);   
}


Image scaleCuda(const Image & image, float scaleX, float scaleY)
{
    Mat2 scaleMat = {
        { scaleX, 0.0f   },
        { 0.0f,   scaleY }
    };

    return transformCuda(image, scaleMat);
}


__device__ int clamp(int x, int a, int b)
{
    if (x < a)
        return a;
    if (x > b)
        return b;
    return x;
}


__global__ void convolve2DKernel(float * input, float * output, int width, int height, float * filter, int size)
{
    int r = (size - 1) / 2;
    float sumR = 0.0f;
    float sumG = 0.0f;
    float sumB = 0.0f;
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    for (int j = y-r; j <= y+r; ++j)
    {
        for (int i = x-r; i <= x+r; ++i)
        {
            float weight = filter[(j-y+r) * size + (i-x+r)];
            int ci = clamp(i, 0, width-1);
            int cj = clamp(j, 0, height-1);

            sumR += input[cj * width*3 + ci*3 + 0] * weight;
            sumG += input[cj * width*3 + ci*3 + 1] * weight;
            sumB += input[cj * width*3 + ci*3 + 2] * weight;
        }
    }

    output[y * width*3 + x*3 + 0] = sumR;
    output[y * width*3 + x*3 + 1] = sumG;
    output[y * width*3 + x*3 + 2] = sumB;
}


__global__ void printFilter(float * filter, int size)
{
    int x = 0;
    int y = 0;
    int r = (size - 1) / 2;
    for (int j = y-r; j <= y+r; ++j)
    {
        for (int i = x-r; i <= x+r; ++i)
        {
            float weight = filter[(i-x+r) * size + (j-y+r)];
            printf("%f ", weight);
        }
        printf("\n");
    }
}


Image convolveImage2DCuda(const Image & image, const FilterType1D & filter)
{
    Image convolved(image.width(), image.height());

    int size = sqrt(filter.size());

    float * input;
    cudaMalloc(&input, image.width() * image.height() * sizeof(float) * 3);
    cudaMemcpy(input, &image(0, 0), image.width() * image.height() * sizeof(float) * 3, cudaMemcpyHostToDevice);
    float * output;
    cudaMalloc(&output, convolved.width() * convolved.height() * sizeof(float) * 3);
    float * filter2D;
    cudaMalloc(&filter2D, size * size * sizeof(float));
    cudaMemcpy(filter2D, &filter[0], size * size * sizeof(float), cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    dim3 gridSize((convolved.width()  + blockSize.x - 1) / blockSize.x,
                  (convolved.height() + blockSize.y - 1) / blockSize.y);

    convolve2DKernel<<<gridSize, blockSize>>>(input, output, image.width(), image.height(), filter2D, size);

    cudaDeviceSynchronize();
    cudaMemcpy(&convolved(0, 0), output, convolved.width() * convolved.height() * sizeof(float) * 3, cudaMemcpyDeviceToHost);

    cudaFree(input);
    cudaFree(output);
    cudaFree(filter2D);

    return convolved;
}


__global__ void convolve1DKernel(float * input, float * output, int width, int height, float * filter, int size, bool vert)
{
    int r = (size - 1) / 2;
    float sumR = 0.0f;
    float sumG = 0.0f;
    float sumB = 0.0f;
    
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (vert)
    {
        int tmp = x;
        x = y;
        y = tmp;
    }
    for (int i = x-r; i <= x+r; ++i)
    {
        float weight = filter[(i-x+r)];
        int ci = clamp(i, 0, width-1);
        if (vert)
        {
            ci = clamp(i, 0, height-1);
            sumR += input[ci * width*3 + y*3 + 0] * weight;
            sumG += input[ci * width*3 + y*3 + 1] * weight;
            sumB += input[ci * width*3 + y*3 + 2] * weight;
        }
        else
        {
            ci = clamp(i, 0, width-1);
            sumR += input[y * width*3 + ci*3 + 0] * weight;
            sumG += input[y * width*3 + ci*3 + 1] * weight;
            sumB += input[y * width*3 + ci*3 + 2] * weight;
        }
    }

    if (vert)
    {
        output[x * width*3 + y*3 + 0] = sumR;
        output[x * width*3 + y*3 + 1] = sumG;
        output[x * width*3 + y*3 + 2] = sumB;
    }
    else
    {
        output[y * width*3 + x*3 + 0] = sumR;
        output[y * width*3 + x*3 + 1] = sumG;
        output[y * width*3 + x*3 + 2] = sumB;
    }
}


Image convolveImage1DCuda(const Image & image, const FilterType1D & filter)
{
    Image middle(image.width(), image.height());

    int size = filter.size();
    
    float * input;
    cudaMalloc(&input, image.width() * image.height() * sizeof(float) * 3);
    cudaMemcpy(input, &image(0, 0), image.width() * image.height() * sizeof(float) * 3, cudaMemcpyHostToDevice);

    float * output;
    cudaMalloc(&output, middle.width() * middle.height() * sizeof(float) * 3);
    
    float * filter1D;
    cudaMalloc(&filter1D, size * sizeof(float));
    cudaMemcpy(filter1D, &filter[0], size * sizeof(float), cudaMemcpyHostToDevice);
    
    dim3 blockSize(16, 16);
    dim3 gridSize((middle.width()  + blockSize.x - 1) / blockSize.x,
                  (middle.height() + blockSize.y - 1) / blockSize.y);

    convolve1DKernel<<<gridSize, blockSize>>>(input, output, image.width(), image.height(), filter1D, size, false);

    cudaDeviceSynchronize();
    
    //cudaMemcpy(input, output, middle.width() * middle.height() * sizeof(float) * 3, cudaMemcpyDeviceToDevice);

    convolve1DKernel<<<gridSize, blockSize>>>(output, input, middle.width(), middle.height(), filter1D, size, true);
    
    cudaDeviceSynchronize();
    
    Image convolved(middle.width(), middle.height());
    cudaMemcpy(&convolved(0, 0), input, convolved.width() * convolved.height() * sizeof(float) * 3, cudaMemcpyDeviceToHost);

    cudaFree(input);
    cudaFree(output);
    cudaFree(filter1D);

    return convolved;
}


Image boxBlurCuda(const Image & image, int radius)
{
    int size = 2 * radius + 1;
    float weight = 1.0f / (size * size);
    FilterType1D boxFilter(size*size, weight);

    return convolveImage2DCuda(image, boxFilter);
}


Image boxBlurSeparableCuda(const Image & image, int radius)
{
    int size = 2 * radius + 1;
    float weight = 1.0f / size;
    FilterType1D boxFilter(size, weight);

    return convolveImage1DCuda(image, boxFilter);
}


Image gaussianBlurCuda(const Image & image, int radius, float stddev)
{
    int size = 2 * radius + 1;
    float s = 2 * stddev * stddev;
    FilterType1D gaussianFilter(size*size, 0.0f);
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float x2 = x - radius;
            float y2 = y - radius;
            float G = expf(-(x2*x2 + y2*y2) / s) / (M_PI * s);
            gaussianFilter[y*size+x] = G;
        }
    }

    return convolveImage2DCuda(image, gaussianFilter);
}


Image gaussianBlurSeparableCuda(const Image & image, int radius, float stddev)
{
    int size = 2 * radius + 1;
    float s = 2 * stddev * stddev;
    FilterType1D gaussianFilter(size, 0.0f);
    for (int x = 0; x < size; ++x)
    {
        float x2 = x - radius;
        float G = expf(-(x2 * x2) / s) / sqrtf(M_PI * s);
        gaussianFilter[x] = G;
    }

    return convolveImage1DCuda(image, gaussianFilter);
}


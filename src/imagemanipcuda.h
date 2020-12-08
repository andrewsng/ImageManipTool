// imagemanipcuda.h
// Andrew S. Ng
// Started: 2020-12-01
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Header for image processing operations in CUDA

#ifndef FILE_IMAGEMANIPCUDA_H_INCLUDED
#define FILE_IMAGEMANIPCUDA_H_INCLUDED

#include "image.h"  // For Image class


// rotateCuda
// Rotates image by given angle.
// Uses CUDA for GPU acceleration.
Image rotateCuda(const Image & image, float angleDeg);


// scaleCuda
// Scales image by given x and y multipliers.
// Uses CUDA for GPU acceleration.
Image scaleCuda(const Image & image, float scaleX, float scaleY);


// skewCuda
// Skews image by given x and y scales.
// Uses CUDA for GPU acceleration.
Image skewCuda(const Image & image, float skewX, float skewY);


// boxBlurCuda
// Blurs image using a 2D box filter of given radius.
// Uses CUDA for GPU acceleration.
Image boxBlurCuda(const Image & image, int radius);


// boxBlurSeparableCuda
// Blurs image using a separable 1D box filter of given radius.
// Uses CUDA for GPU acceleration.
Image boxBlurSeparableCuda(const Image & image, int radius);


// gaussianBlurCuda
// Blurs image using a 2D gaussian filter of given radius and std. deviation.
// Uses CUDA for GPU acceleration.
Image gaussianBlurCuda(const Image & image, int radius, float stddev);


// gaussianBlurSeparableCuda
// Blurs image using a separable 1D gaussian filter of given radius and std. deviation.
// Uses CUDA for GPU acceleration.
Image gaussianBlurSeparableCuda(const Image & image, int radius, float stddev);


#endif  // #ifndef FILE_IMAGEMANIPCUDA_H_INCLUDED


// imagemanip.h
// Andrew S. Ng
// Started: 2020-10-23
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Header for image processing operations

#ifndef FILE_IMAGEMANIP_H_INCLUDED
#define FILE_IMAGEMANIP_H_INCLUDED

#include "image.h"  // For Image class


// rotate
// Rotates image by given angle.
Image rotate(const Image & image, float angleDeg);


// scale
// Scales image by given x and y multipliers.
Image scale(const Image & image, float scaleX, float scaleY);


// skew
// Skews image by given x and y scales.
Image skew(const Image & image, float skewX, float skewY);


// boxBlur
// Blurs image using a 2D box filter of given radius.
Image boxBlur(const Image & image, int radius);


// boxBlurSeparable
// Blurs image using a separable 1D box filter of given radius.
Image boxBlurSeparable(const Image & image, int radius);

// gaussianBlur
// Blurs image using a 2D gaussian filter of given radius and std. deviation.
Image gaussianBlur(const Image & image, int radius, float stddev);


// gaussianBlurSeparable
// Blurs image using a separable 1D gaussian filter of given radius and std. deviation.
Image gaussianBlurSeparable(const Image & image, int radius, float stddev);


#endif  // #ifndef FILE_IMAGEMANIP_H_INCLUDED


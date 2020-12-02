#ifndef FILE_IMAGEMANIP_H_INCLUDED
#define FILE_IMAGEMANIP_H_INCLUDED

#include "image.h"


Image rotate(const Image & image, float angleDeg);

Image scale(const Image & image, float scaleX, float scaleY);

Image skew(const Image & image, float skewX, float skewY);

Image boxBlur(const Image & image, int radius);

Image boxBlurSeparable(const Image & image, int radius);

Image gaussianBlur(const Image & image, int radius, float stddev);

Image gaussianBlurSeparable(const Image & image, int radius, float stddev);


#endif  // #ifndef FILE_IMAGEMANIP_H_INCLUDED


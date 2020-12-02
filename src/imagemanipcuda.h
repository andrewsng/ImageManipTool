#ifndef FILE_IMAGEMANIPCUDA_H_INCLUDED
#define FILE_IMAGEMANIPCUDA_H_INCLUDED

#include "image.h"


Image rotateCuda(const Image & image, float angleDeg);

Image scaleCuda(const Image & image, float scaleX, float scaleY);

Image boxBlurCuda(const Image & image, int radius);

Image boxBlurSeparableCuda(const Image & image, int radius);

Image gaussianBlurCuda(const Image & image, int radius, float stddev);

Image gaussianBlurSeparableCuda(const Image & image, int radius, float stddev);


#endif  // #ifndef FILE_IMAGEMANIPCUDA_H_INCLUDED


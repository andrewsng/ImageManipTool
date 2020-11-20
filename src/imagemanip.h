#ifndef FILE_IMAGEMANIP_H_INCLUDED
#define FILE_IMAGEMANIP_H_INCLUDED

#include "image.h"


Image rotate(const Image & image, float angleDeg);

Image scale(const Image & image, float scaleX, float scaleY);


#endif  // #ifndef FILE_IMAGEMANIP_H_INCLUDED


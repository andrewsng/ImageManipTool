#ifndef FILE_IMAGEMANIP_H_INCLUDED
#define FILE_IMAGEMANIP_H_INCLUDED

#include "ppmimage.h"


PPMImage rotate(const PPMImage & image, float angleDeg);

PPMImage scale(const PPMImage & image, float scaleX, float scaleY);


#endif  // #ifndef FILE_IMAGEMANIP_H_INCLUDED


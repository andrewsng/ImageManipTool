#include "imagemanip.h"
#include "mat2.h"
#include "vec2.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>


PPMImage transform(const PPMImage & image, Mat2 transform)
{
    Mat2 transformInv = transform.inverse();

    float maxX = (float)image.width() - 1.0f;
    float maxY = (float)image.height() - 1.0f;
    Vec2 topLeft(0.0f, maxY);
    Vec2 topRight(maxX, maxY);
    Vec2 botLeft(0.0f, 0.0f);
    Vec2 botRight(maxX, 0.0f);

    topLeft = transform * topLeft;
    topRight = transform * topRight;
    botLeft = transform * botLeft;
    botRight = transform * botRight;

    int newMaxX = std::max({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    int newMinX = std::min({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    int newMaxY = std::max({ topLeft.y, topRight.y, botLeft.y, botRight.y });
    int newMinY = std::min({ topLeft.y, topRight.y, botLeft.y, botRight.y });

    PPMImage transformed(newMaxX - newMinX + 1, newMaxY - newMinY + 1);

    for (int y = 0; y < transformed.height(); ++y)
    {
        for (int x = 0; x < transformed.width(); ++x)
        {
            Vec2 samplePos = transformInv * Vec2(newMinX + x, newMinY + y);

            if (samplePos.x < -0.1f || (float)(image.width()) <= samplePos.x
             || samplePos.y < -0.1f || (float)(image.height()) <= samplePos.y)
                transformed.setColor(x, transformed.height() - y - 1, 0, 0, 0);
            else {
                PPMImage::pixel color = image.getColor(samplePos.x, (float)(image.height() - 1) - samplePos.y);
                transformed.setColor(x, transformed.height() - y - 1, color.r, color.g, color.b);
            }
        }
    }
    return transformed;
}


PPMImage rotate(const PPMImage & image, float angleDeg)
{
    float angleRad = M_PI * angleDeg / 180.0f;
    float cosAngle = cosf(angleRad);
    float sinAngle = sinf(angleRad);
    Mat2 rotMat = {
        { cosAngle, -sinAngle },
        { sinAngle,  cosAngle }
    };

    return transform(image, rotMat);   
}


PPMImage scale(const PPMImage & image, float scaleX, float scaleY)
{
    Mat2 scaleMat = {
        { scaleX, 0.0f   },
        { 0.0f,   scaleY }
    };

    return transform(image, scaleMat);
}


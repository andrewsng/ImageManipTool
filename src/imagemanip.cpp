#include "imagemanip.h"
#include "mat2.h"
#include "vec2.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <cmath>


Image transform(const Image & image, Mat2 transform)
{
    Mat2 transformInv = transform.inverse();

    float maxX = float(image.width() - 1);
    float maxY = float(image.height() - 1);
    // Vec2 topLeft(0.0f, maxY);
    // Vec2 topRight(maxX, maxY);
    // Vec2 botLeft(0.0f, 0.0f);
    // Vec2 botRight(maxX, 0.0f);
    Vec2 topLeft(0.5f, maxY + 0.5f);
    Vec2 topRight(maxX + 0.5f, maxY + 0.5f);
    Vec2 botLeft(0.5f, 0.5f);
    Vec2 botRight(maxX + 0.5f, 0.5f);

    topLeft = transform * topLeft;
    topRight = transform * topRight;
    botLeft = transform * botLeft;
    botRight = transform * botRight;

    float newMaxX = std::max({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMinX = std::min({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMaxY = std::max({ topLeft.y, topRight.y, botLeft.y, botRight.y });
    float newMinY = std::min({ topLeft.y, topRight.y, botLeft.y, botRight.y });

    Image transformed(newMaxX - newMinX + 1, newMaxY - newMinY + 1);

    for (int y = 0; y < transformed.height(); ++y)
    {
        for (int x = 0; x < transformed.width(); ++x)
        {
            Vec2 samplePos = transformInv * Vec2(newMinX + x, newMinY + y);

            if (0.0f <= samplePos.x && samplePos.x <= float(image.width()) &&
                0.0f <= samplePos.y && samplePos.y <= float(image.height()))
            {
                Image::Pixel color = image.getColor(samplePos.x, float(image.height() - 1) - samplePos.y);
                transformed.setColor(x, transformed.height() - y - 1, color.r, color.g, color.b);
            }
        }
    }
    return transformed;
}


Image rotate(const Image & image, float angleDeg)
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


Image scale(const Image & image, float scaleX, float scaleY)
{
    Mat2 scaleMat = {
        { scaleX, 0.0f   },
        { 0.0f,   scaleY }
    };

    return transform(image, scaleMat);
}


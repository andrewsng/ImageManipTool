#ifndef FILE_IMAGEMANIP_H_INCLUDED
#define FILE_IMAGEMANIP_H_INCLUDED


#include <glm/glm.hpp>
#include "ppmimage.h"

#include <algorithm>


PPMImage rotate(const PPMImage & image, float angleDeg)
{
    float angleRad = glm::radians(angleDeg);
    float cosAngle = glm::cos(angleRad);
    float sinAngle = glm::sin(angleRad);
    glm::mat2 rotMat = {
        { cosAngle, sinAngle },
        { -sinAngle, cosAngle }
    };
    glm::mat2 rotMatInv = glm::inverse(rotMat);

    float maxX = (float)image.width() - 1.0f;
    float maxY = (float)image.height() - 1.0f;
    glm::vec2 topLeft(0.0f, maxY);
    glm::vec2 topRight(maxX, maxY);
    glm::vec2 botLeft(0.0f, 0.0f);
    glm::vec2 botRight(maxX, 0.0f);

    topLeft = rotMat * topLeft;
    topRight = rotMat * topRight;
    botLeft = rotMat * botLeft;
    botRight = rotMat * botRight;

    int newMaxX = std::max({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    int newMinX = std::min({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    int newMaxY = std::max({ topLeft.y, topRight.y, botLeft.y, botRight.y });
    int newMinY = std::min({ topLeft.y, topRight.y, botLeft.y, botRight.y });

    PPMImage rotated(newMaxX - newMinX + 1, newMaxY - newMinY + 1);

    for (int y = 0; y < rotated.height(); ++y)
    {
        for (int x = 0; x < rotated.width(); ++x)
        {
            glm::vec2 samplePos = rotMatInv * glm::vec2(newMinX + x, newMinY + y);

            if (samplePos.x < -0.1f || (float)(image.width()) <= samplePos.x
             || samplePos.y < -0.1f || (float)(image.height()) <= samplePos.y)
                rotated.setColor(x, rotated.height() - y - 1, 0, 0, 0);
            else {
                PPMImage::pixel color = image.getColor(samplePos.x, (float)(image.height() - 1) - samplePos.y);
                rotated.setColor(x, rotated.height() - y - 1, color.r, color.g, color.b);
            }
        }
    }
    return rotated;
}


#endif  // #ifndef FILE_IMAGEMANIP_H_INCLUDED
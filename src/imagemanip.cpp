#include "imagemanip.h"
#include "mat2.h"
#include "vec2.h"

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

    float newMaxX = max({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMinX = min({ topLeft.x, topRight.x, botLeft.x, botRight.x });
    float newMaxY = max({ topLeft.y, topRight.y, botLeft.y, botRight.y });
    float newMinY = min({ topLeft.y, topRight.y, botLeft.y, botRight.y });

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


Image::Pixel convolvePixel2D(const Image & image, const FilterType2D & filter, int x, int y)
{
    int r = (filter.size() - 1) / 2;
    Image::Pixel sum;
    for (int j = y-r; j <= y+r; ++j)
    {
        for (int i = x-r; i <= x+r; ++i)
        {
            float weight = filter[i-x+r][j-y+r];

            int clampI = max(0, min(i, image.width() - 1));
            int clampJ = max(0, min(j, image.height() - 1));

            Image::Pixel val = image.getColor(clampI, clampJ);
            sum.r += val.r * weight;
            sum.g += val.g * weight;
            sum.b += val.b * weight;
        }
    }

    return sum;
}


Image convolveImage2D(const Image & image, const FilterType2D & filter)
{
    Image convolved(image.width(), image.height());

    for (int y = 0; y < convolved.height(); ++y)
    {
        for (int x = 0; x < convolved.width(); ++x)
        {
            convolved.setColor(x, y, convolvePixel2D(image, filter, x, y));
        }
    }

    return convolved;
}


Image::Pixel convolvePixel1D(const Image & image, const FilterType1D & filter, int x, int y, bool vert)
{
    int r = (filter.size() - 1) / 2;
    Image::Pixel sum;

    if (vert)
        swap(x, y);

    for (int i = x-r; i <= x+r; ++i)
    {
        float weight = filter[i-x+r];

        int clampI;
        Image::Pixel val;
        if (vert)
        {
            clampI = max(0, min(i, image.height() - 1));
            val = image.getColor(y, clampI);
        }
        else
        {
            clampI = max(0, min(i, image.width() - 1));
            val = image.getColor(clampI, y);
        }
        sum.r += val.r * weight;
        sum.g += val.g * weight;
        sum.b += val.b * weight;
    }

    return sum;
}


Image convolveImageSeparable(const Image & image, const FilterType1D & filter)
{
    Image middle(image.width(), image.height());
    for (int y = 0; y < middle.height(); ++y)
    {
        for (int x = 0; x < middle.width(); ++x)
        {
            middle.setColor(x, y, convolvePixel1D(image, filter, x, y, false));
        }
    }

    Image convolved(middle.width(), middle.height());
    for (int y = 0; y < convolved.height(); ++y)
    {
        for (int x = 0; x < convolved.width(); ++x)
        {
            convolved.setColor(x, y, convolvePixel1D(middle, filter, x, y, true));
        }
    }

    return convolved;
}


Image boxBlur(const Image & image, int radius)
{
    int size = 2 * radius + 1;
    float weight = 1.0f / (size * size);
    FilterType2D boxFilter(size, FilterType1D(size, weight));

    return convolveImage2D(image, boxFilter);
}


Image boxBlurSeparable(const Image & image, int radius)
{
    int size = 2 * radius + 1;
    float weight = 1.0f / size;
    FilterType1D boxFilter(size, weight);

    return convolveImageSeparable(image, boxFilter);
}


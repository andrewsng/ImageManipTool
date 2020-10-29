#include "ppmimage.h"
#include "imagemanip.h"

#include <string>


int main()
{
    PPMImage image(800, 600);

    image.read("../test/house_2.ppm");
    image = rotate(image, 60.0f);
    image.write("TestImage.ppm");

    return 0;
}
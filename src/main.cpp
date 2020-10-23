#include "ppmimage.h"
#include "imagemanip.h"

#include <string>


int main()
{
    PPMImage framebuffer(800, 600);

    framebuffer.read("../test/house_2.ppm");
    framebuffer = rotate(framebuffer, -90.0f);
    framebuffer.write("TestImage.ppm");

    return 0;
}
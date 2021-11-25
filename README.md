# CS 301 Project:

## Image Processing using CUDA

For this project, I implemented a few image editing tools commonly seen in software such as Photoshop, or GIMP, 
and then sped up those operations using GPU acceleration with CUDA.

## Transformations

Here is an example of the rotation operation. Instead of preserving the original image size,
the size depends entirely on where the transformed image ends up, so be careful doing repeated operations, as it might blow up the image size.

![House](https://github.com/andrewsng/ImageManipTool/blob/main/test/output/house_2.png?raw=true)
![RotatedHouse](https://github.com/andrewsng/ImageManipTool/blob/main/test/output/house_2_rotate.png?raw=true)

## Convolutions/Filtering

Here is the input and output of a Gaussian blur convolution. The two filters I added, box blur and gaussian blur, both have the option to 
use their separable 1D filters, which convolve the image much faster as filter sizes grow.

![Building](https://github.com/andrewsng/ImageManipTool/blob/main/test/output/west_2.png?raw=true)
![BlurredBuilding](https://github.com/andrewsng/ImageManipTool/blob/main/test/output/west_2_gauss.png?raw=true)

## Building

I had trouble getting Cmake to work with CUDA, but it can be simply compiled using Nvidia's CUDA compiler:

`nvcc -std=c++17 -O3 main.cpp image.cpp imagemanip.cpp imagemanipcuda.cu`

## Running

Currently, the program only supports files of the "P6" .ppm image format.
The program is executed using command line arguments in this format:

`./a.out inputfile.ppm outputfile.ppm (optional -c for CUDA) [image operation] [operation parameters]`

Where the operations and parameters are:

Rotation:                   `-rotate angleDegrees`

Scale/Resize:               `-scale scaleX scaleY`

Skew/Shear:                 `-skew skewX skewY`

Box Blur:                   `-box radius`

Box Blur (Separable):       `-boxSep radius`

Note: Typically for Gaussian blur, the radius is 3 times the stddev, and 3 and 1 is a good starting point.

Gaussian Blur:              `-gaussian radius stddev`

Gaussian Blur (Separable):  `-gaussianSep radius stddev`


## Sources

[Test images from cs.cornell.edu](http://www.cs.cornell.edu/courses/cs664/2003fa/images/)

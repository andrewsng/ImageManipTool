#include "image.h"
#include "imagemanip.h"
#include "mat2.h"
#include "vec2.h"

#include <iostream>
#include <string>
#include <chrono>


int main()
{
    Image image(800, 600);

    image.readPPM("../test/house_2.ppm");

    auto start = std::chrono::steady_clock::now();
    image = rotate(image, 90.0f);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::nano> elapsed = end - start;
    std::cout << "Elapsed time (rotate): " << elapsed.count() / (image.height() * image.width()) << " ns/px" << std::endl;

    start = std::chrono::steady_clock::now();
    image = scale(image, 2.0f, 3.0f);
    end = std::chrono::steady_clock::now();
    elapsed = end - start;
    std::cout << "Elapsed time (scale): " << elapsed.count() / (image.height() * image.width()) << " ns/px" << std::endl;
    
    image.writePPM("TestImage.ppm");

    return 0;
}

// int main()
// {
//     Mat2 matrix = {
//         { 1.5f, 2.5f },
//         { 3.5f, 4.5f }
//     };
//     for (int i = 0; i < 2; ++i)
//     {
//         for (int j = 0; j < 2; ++j)
//         {
//             std::cout << "matrix[" << i << "][" << j << "] = " << matrix[i][j] << "\n";
//         }
//     }

//     Mat2 matrix2 = matrix.inverse();
//     for (int i = 0; i < 2; ++i)
//     {
//         for (int j = 0; j < 2; ++j)
//         {
//             std::cout << "matrix2[" << i << "][" << j << "] = " << matrix2[i][j] << "\n";
//         }
//     }

//     Vec2 vector(1.0f, 2.0f);
//     std::cout << "vector.x = " << vector.x << "\n";
//     std::cout << "vector.y = " << vector.y << "\n";

//     Vec2 vector2 = matrix * vector;
//     std::cout << "vector2.x = " << vector2.x << "\n";
//     std::cout << "vector2.y = " << vector2.y << "\n";
// }


#include "image.h"
#include "imagemanip.h"
#include "imagemanipcuda.h"
#include "mat2.h"
#include "vec2.h"

#include <iostream>
#include <string>
#include <chrono>


int main()
{
    Image image(800, 600);

    image.readPPM("../test/west_2.ppm");

    Image output;

    // image = scale(image, 0.5f, 0.5f);

    int n = 0;
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            rotate(image, 60);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (rotate 60): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = rotate(image, -120);
        output.writePPM("../out/0_rotate.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            rotateCuda(image, 60);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (rotate cuda 60): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = rotateCuda(image, 60);
        output.writePPM("../out/1_rotate_cuda.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            scale(image, 1.5f, 0.75f);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (scale 2x): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = scale(image, 1.25f, 1.75f);
        output.writePPM("../out/2_scale.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            scaleCuda(image, 1.5f, 0.75f);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (scale cuda 2x): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = scaleCuda(image, 1.5f, 0.75f);
        output.writePPM("../out/3_scale_cuda.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < 1; ++i)
        {
            skew(image, 0.25f, 0.75f);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (skew 2X): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = skew(image, 0.125f, 0.25f);
        output.writePPM("../out/12_skew.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            boxBlur(image, 15);
            std::cout << i << '\n';
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (box blur): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = boxBlur(image, 15);
        output.writePPM("../out/4_box.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            boxBlurCuda(image, 15);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (box cuda): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = boxBlurCuda(image, 15);
        output.writePPM("../out/5_box_cuda.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            boxBlurSeparable(image, 15);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (box sep): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = boxBlurSeparable(image, 30);
        output.writePPM("../out/6_box_sep.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            boxBlurSeparableCuda(image, 15);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (box cuda sep): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = boxBlurSeparableCuda(image, 15);
        output.writePPM("../out/7_box_sep_cuda.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            gaussianBlur(image, 15, 5);
            std::cout << i << '\n';
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (gauss blur): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = gaussianBlur(image, 15, 5);
        output.writePPM("../out/8_gauss.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            gaussianBlurCuda(image, 15, 5);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (gauss cuda): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = gaussianBlurCuda(image, 15, 5);
        output.writePPM("../out/9_gauss_cuda.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            gaussianBlurSeparable(image, 15, 5);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (gauss sep): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = gaussianBlurSeparable(image, 15, 5);
        output.writePPM("../out/10_gauss_sep.ppm");
    }
    {
        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < n; ++i)
        {
            gaussianBlurSeparableCuda(image, 15, 5);
        }
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::nano> elapsed = end - start;
        std::cout << "Avg time (gauss sep cuda): " << elapsed.count() / (n * image.height() * image.width()) << " ns/px" << std::endl;
        output = gaussianBlurSeparableCuda(image, 60, 20);
        output.writePPM("../out/11_gauss_sep_cuda.ppm");
    }
    
    // output.writePPM("TestImage.ppm");

    return 0;
}


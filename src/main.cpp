// main.cpp
// Andrew S. Ng
// Started: 2020-10-23
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Main for image manip program

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

    image.readPPM("../test/cat.ppm");

    Image output;

    // image = scale(image, 0.5f, 0.5f);

    int n = 10;
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            rotate(image, 60);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (rotate 60): " << time / n << " ns/px" << std::endl;
        output = rotate(image, 60);
        output.writePPM("../out/0_rotate.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            rotateCuda(image, 60);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (rotate cuda 60): " << time / n << " ns/px" << std::endl;
        output = rotateCuda(image, 60);
        output.writePPM("../out/1_rotate_cuda.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            scale(image, 1.25f, 1.75f);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (scale 2x): " << time / n << " ns/px" << std::endl;
        output = scale(image, 1.25f, 1.75f);
        output.writePPM("../out/2_scale.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            scaleCuda(image, 1.25f, 1.75f);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (scale cuda 2x): " << time / n << " ns/px" << std::endl;
        output = scaleCuda(image, 1.25f, 1.75f);
        output.writePPM("../out/3_scale_cuda.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            skew(image, 0.125f, 0.25f);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (skew 2X): " << time / n << " ns/px" << std::endl;
        output = skew(image, 0.125f, 0.25f);
        output.writePPM("../out/12_skew.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            boxBlur(image, 15);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (box blur): " << time / n << " ns/px" << std::endl;
        output = boxBlur(image, 15);
        output.writePPM("../out/4_box.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            boxBlurCuda(image, 15);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (box cuda): " << time / n << " ns/px" << std::endl;
        output = boxBlurCuda(image, 15);
        output.writePPM("../out/5_box_cuda.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            boxBlurSeparable(image, 15);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (box sep): " << time / n << " ns/px" << std::endl;
        output = boxBlurSeparable(image, 15);
        output.writePPM("../out/6_box_sep.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            boxBlurSeparableCuda(image, 15);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (box cuda sep): " << time / n << " ns/px" << std::endl;
        output = boxBlurSeparableCuda(image, 15);
        output.writePPM("../out/7_box_sep_cuda.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            gaussianBlur(image, 15, 5);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (gauss blur): " << time / n << " ns/px" << std::endl;
        output = gaussianBlur(image, 15, 5);
        output.writePPM("../out/8_gauss.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            gaussianBlurCuda(image, 15, 5);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (gauss cuda): " << time / n << " ns/px" << std::endl;
        output = gaussianBlurCuda(image, 15, 5);
        output.writePPM("../out/9_gauss_cuda.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            gaussianBlurSeparable(image, 15, 5);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (gauss sep): " << time / n << " ns/px" << std::endl;
        output = gaussianBlurSeparable(image, 15, 5);
        output.writePPM("../out/10_gauss_sep.ppm");
    }
    {
        double time = 0.0;
        for (int i = 0; i < n; ++i)
        {
            auto start = std::chrono::steady_clock::now();
            gaussianBlurSeparableCuda(image, 15, 5);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> elapsed = end - start;
            time += elapsed.count() / (image.height() * image.width());
        }
        std::cout << "Avg time (gauss sep cuda): " << time / n << " ns/px" << std::endl;
        output = gaussianBlurSeparableCuda(image, 15, 5);
        output.writePPM("../out/11_gauss_sep_cuda.ppm");
    }
    
    // output.writePPM("TestImage.ppm");

    return 0;
}


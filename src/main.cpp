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

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <chrono>


int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        cerr << "ERROR: Not enough arguments" << endl;
        return -1;
    }

    Image input;
    if (!input.readPPM(argv[1]))
    {
        cerr << "ERROR: Input image could not be read" << endl;
        return -1;
    }

    int k = 0;
    bool cuda = false;
    if (argv[3] == string("-c"))
    {
        cuda = true;
        k = 1;
        if (argc < 5)
        {
            cerr << "ERROR: Not enough arguments" << endl;
            return -1;
        }
    }

    if (argv[3+k] == string("-rotate") && argc >= 5+k)
    {
        float angle;
        istringstream iss(argv[4+k]);
        if (!(iss >> angle))
        {
            cerr << "ERROR: Invalid rotation angle" << endl;
            return -1;
        }

        if (cuda)
            input = rotateCuda(input, angle);
        else
            input = rotate(input, angle);
    }
    else if (argv[3+k] == string("-scale") && argc >= 6+k)
    {
        float x, y;
        {
            istringstream iss(argv[4+k]);
            if (!(iss >> x))
            {
                cerr << "ERROR: Invalid x scale" << endl;
                return -1;
            }
        }
        {
            istringstream iss(argv[5+k]);
            if (!(iss >> y))
            {
                cerr << "ERROR: Invalid y scale" << endl;
                return -1;
            }
        }

        if (cuda)
            input = scaleCuda(input, x, y);
        else
            input = scale(input, x, y);
    }
    else if (argv[3+k] == string("-skew") && argc >= 6+k)
    {
        float x, y;
        {
            istringstream iss(argv[4+k]);
            if (!(iss >> x))
            {
                cerr << "ERROR: Invalid x scale" << endl;
                return -1;
            }
        }
        {
            istringstream iss(argv[5+k]);
            if (!(iss >> y))
            {
                cerr << "ERROR: Invalid y scale" << endl;
                return -1;
            }
        }

        if (cuda)
            input = skewCuda(input, x, y);
        else
            input = skew(input, x, y);
    }
    else if (argv[3+k] == string("-box") && argc >= 5+k)
    {
        int radius;
        istringstream iss(argv[4+k]);
        if (!(iss >> radius))
        {
            cerr << "ERROR: Invalid filter radius" << endl;
            return -1;
        }

        if (cuda)
            input = boxBlurCuda(input, radius);
        else
            input = boxBlur(input, radius);
    }
    else if (argv[3+k] == string("-boxSep") && argc >= 5+k)
    {
        int radius;
        istringstream iss(argv[4+k]);
        if (!(iss >> radius))
        {
            cerr << "ERROR: Invalid filter radius" << endl;
            return -1;
        }

        if (cuda)
            input = boxBlurSeparableCuda(input, radius);
        else
            input = boxBlurSeparable(input, radius);
    }
    else if (argv[3+k] == string("-gaussian") && argc >= 6+k)
    {
        int radius;
        float stddev;
        {
            istringstream iss(argv[4+k]);
            if (!(iss >> radius))
            {
                cerr << "ERROR: Invalid filter radius" << endl;
                return -1;
            }
        }
        {
            istringstream iss(argv[5+k]);
            if (!(iss >> stddev))
            {
                cerr << "ERROR: Invalid standard deviation" << endl;
                return -1;
            }
        }

        if (cuda)
            input = gaussianBlurCuda(input, radius, stddev);
        else
            input = gaussianBlur(input, radius, stddev);
    }
    else if (argv[3+k] == string("-gaussianSep") && argc >= 6+k)
    {
        int radius;
        float stddev;
        {
            istringstream iss(argv[4+k]);
            if (!(iss >> radius))
            {
                cerr << "ERROR: Invalid filter radius" << endl;
                return -1;
            }
        }
        {
            istringstream iss(argv[5+k]);
            if (!(iss >> stddev))
            {
                cerr << "ERROR: Invalid standard deviation" << endl;
                return -1;
            }
        }

        if (cuda)
            input = gaussianBlurSeparableCuda(input, radius, stddev);
        else
            input = gaussianBlurSeparable(input, radius, stddev);
    }
    else
    {
        cerr << "ERROR: Invalid arguments" << endl;
        return -1;
    }

    if (!input.writePPM(argv[2]))
    {
        cerr << "ERROR: Output image could not be written" << endl;
        return -1;
    }
    
    return 0;
}


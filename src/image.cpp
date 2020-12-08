// image.cpp
// Andrew S. Ng
// Started: 2020-12-02
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Source for Image class

#include "image.h"

#include <iostream>
using std::cerr;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <string>
using std::string;
#include <sstream>
using std::istringstream;
#include <vector>
using std::vector;
#include <utility>
using std::move;
#include <cstddef>
using std::size_t;
#include <cstdint>
using std::uint8_t;
#include <limits>


// readPPM
// (see header.)
bool Image::readPPM(string filename)
{
    // Open image file
    ifstream fin(filename, std::ios::binary);
    if (!fin)
    {
        cerr << "ERROR: Input file could not be opened." << endl;
        return false;
    }

    // Read PPM file header
    string type;
    int width, height, maxVal;
    if (!getString(fin, type) ||
        !getInt(fin, width)   ||
        !getInt(fin, height)  ||
        !getInt(fin, maxVal))
    {
        cerr << "ERROR: Could not read PPM file header." << endl;
        return false;
    }

    // Validate PPM file header values
    if (type != "P6" ||
        width < 0    || 
        height < 0   ||
        maxVal != 255)
    {
        cerr << "ERROR: PPM file properties are incompatible." << endl;
        return false;
    }

    // Reads binary file data into buffer of uint8_t's.
    vector<uint8_t> pixels(width * height * 3, 0);
    fin.get();
    fin.read((char *)&pixels[0], pixels.size() * sizeof(uint8_t));
    if (!fin)
    {
        cerr << "ERROR: Could not read image data." << endl;
        return false;
    }

    // Converts uint8_t data into a buffer of floats
    vector<Pixel> data(width * height, Pixel());
    float scale = 1 / 255.0f;
    for (size_t i = 0; i < data.size(); ++i)
    {
        data[i].r = pixels[i * 3 + 0] * scale;
        data[i].g = pixels[i * 3 + 1] * scale;
        data[i].b = pixels[i * 3 + 2] * scale;
    }

    // Move values to object data members
    _width = width;
    _height = height;
    _data = move(data);

    return true;
}


// writePPM
// (see header.)
bool Image::writePPM(string filename) const
{
    // Open image file
    ofstream fout(filename, std::ios::binary);
    if (!fout)
    {
        cerr << "ERROR: Output file could not be opened." << endl;
        return false;
    }

    // Write PPM file header
    fout << "P6" << '\n'
            << width() << ' ' << height() << '\n'
            << 255 << '\n';
    if (!fout)
    {
        cerr << "ERROR: Could not write PPM header to file." << endl;
        return false;
    }

    // Convert float data into a buffer of uint8_t's
    vector<uint8_t> pixels(width() * height() * 3, 0);
    for (size_t i = 0; i < _data.size(); ++i)
    {
        pixels[i * 3 + 0] = (uint8_t)(_data[i].r * 255);
        pixels[i * 3 + 1] = (uint8_t)(_data[i].g * 255);
        pixels[i * 3 + 2] = (uint8_t)(_data[i].b * 255);
    }
    
    // Write binary file data from buffer of uint8_t's
    fout.write((char *)&pixels[0], pixels.size() * sizeof(uint8_t));
    if (!fout)
    {
        cerr << "ERROR: Could not write image data to file." << endl;
        return false;
    }

    return true;
}


// getString
// (see header.)
bool Image::getString(ifstream & ifs, string & str) const
{
    while (true)
    {
        ifs >> str;
        if (!ifs)
            return false;

        if (str[0] == '#')
        {
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            str.clear();
            continue;
        }
        return true;
    }
}


// getInt
// (see header.)
bool Image::getInt(ifstream & ifs, int & num) const
{
    string str;
    if (!getString(ifs, str))
        return false;
    istringstream iss(str);
    iss >> num;
    return true;
}
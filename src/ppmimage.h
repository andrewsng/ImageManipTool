#ifndef FILE_PPMIMAGE_H_INCLUDED
#define FILE_PPMIMAGE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstddef> // For std::size_t
#include <cstdint> // For std::uint8_t
#include <limits>


class PPMImage
{

public:

    struct pixel
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
    };

public:

    PPMImage(int width, int height)
        :_type("P6"),_width(width),_height(height), _maxVal(255),
         _pixels(width * height, pixel())
    {}

    void setColor(int x, int y, int r, int g, int b)
    {
        pixel & px = _pixels[x + (y * _width)];
        px.r = r;
        px.g = g;
        px.b = b;
    }

    pixel getColor(int x, int y) const
    {
        return _pixels[x + (y * _width)];
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    bool read(std::string filename)
    {
        std::ifstream fin(filename, std::ios::binary);
        if (!fin)
        {
            std::cerr << "ERROR: Input file could not be opened." << std::endl;
            return false;
        }

        std::string type;
        int width, height, maxVal;
        if (!getString(fin, type)
         || !getInt(fin, width)
         || !getInt(fin, height)
         || !getInt(fin, maxVal))
        {
            std::cerr << "ERROR: Could not read from file." << std::endl;
            return false;
        }

        std::vector<pixel> pixels(width * height, pixel());
        fin.get();
        fin.read((char*)&pixels[0], pixels.size() * sizeof(pixel));
        if (!fin)
        {
            std::cerr << "ERROR: Could not read from file." << std::endl;
            return false;
        }

        _type = type;
        _width = width;
        _height = height;
        _maxVal = maxVal;
        _pixels = std::move(pixels);

        return true;
    }

    bool write(std::string filename) const
    {
        std::ofstream fout(filename, std::ios::binary);
        if (!fout)
        {
            std::cerr << "ERROR: Output file could not be opened." << std::endl;
            return false;
        }

        fout << _type << '\n'
             << _width << ' ' << _height << '\n'
             << _maxVal << '\n';
        if (!fout)
        {
            std::cerr << "ERROR: Could not write to file." << std::endl;
            return false;
        }
        
        fout.write((char *)&_pixels[0],_pixels.size() * sizeof(pixel));
        if (!fout)
        {
            std::cerr << "ERROR: Could not write to file." << std::endl;
            return false;
        }

        return true;
    }

private:

    bool getString(std::ifstream & ifs, std::string & str)
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

    bool getInt(std::ifstream & ifs, int & num)
    {
        std::string str;
        if (!getString(ifs, str))
            return false;
        std::istringstream iss(str);
        iss >> num;
        return true;
    }

private:

    std::string _type;
    int _width;
    int _height;
    int _maxVal;
    std::vector<pixel> _pixels;

};


#endif  // #indef FILE_PPMIMAGE_H_INCLUDED
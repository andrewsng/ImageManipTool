#ifndef FILE_IMAGE_H_INCLUDED
#define FILE_IMAGE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstddef> // For std::size_t
#include <cstdint> // For std::uint8_t
#include <limits>


class Image
{

public:

    struct Pixel
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
    };

public:

    Image()
        :_width(800),_height(600),
         _data(width() * height(), Pixel())
    {}

    Image(int width, int height)
        :_width(width),_height(height),
         _data(width * height, Pixel())
    {}

    ~Image() = default;
    Image(const Image & other) = default;
    Image & operator=(const Image & other) = default;
    Image(Image && other) = default;
    Image & operator=(Image && other) = default;

public:

    void setColor(int x, int y, float r, float g, float b)
    {
        Pixel & px = _data[x + (y * width())];
        px.r = r;
        px.g = g;
        px.b = b;
    }

    void setColor(int x, int y, Image::Pixel px)
    {
        _data[x + (y * width())] = px;
    }

    Pixel getColor(int x, int y) const
    {
        return _data[x + (y * width())];
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    bool readPPM(std::string filename)
    {
        std::ifstream fin(filename, std::ios::binary);
        if (!fin)
        {
            std::cerr << "ERROR: Input file could not be opened." << std::endl;
            return false;
        }

        std::string type;
        int width, height, maxVal;
        if (!getString(fin, type) ||
            !getInt(fin, width)   ||
            !getInt(fin, height)  ||
            !getInt(fin, maxVal))
        {
            std::cerr << "ERROR: Could not read PPM file header." << std::endl;
            return false;
        }

        if (type != "P6" ||
            width < 0    || 
            height < 0   ||
            maxVal != 255)
        {
            std::cerr << "ERROR: PPM file properties are incompatible." << std::endl;
            return false;
        }

        std::vector<std::uint8_t> pixels(width * height * 3, 0);
        fin.get();
        fin.read((char *)&pixels[0], pixels.size() * sizeof(std::uint8_t));
        if (!fin)
        {
            std::cerr << "ERROR: Could not read image data." << std::endl;
            return false;
        }

        std::vector<Pixel> data(width * height, Pixel());
        float scale = 1 / 255.0f;
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            data[i].r = pixels[i * 3 + 0] * scale;
            data[i].g = pixels[i * 3 + 1] * scale;
            data[i].b = pixels[i * 3 + 2] * scale;
        }

        _width = width;
        _height = height;
        _data = std::move(data);

        return true;
    }

    bool writePPM(std::string filename) const
    {
        std::ofstream fout(filename, std::ios::binary);
        if (!fout)
        {
            std::cerr << "ERROR: Output file could not be opened." << std::endl;
            return false;
        }

        fout << "P6" << '\n'
             << width() << ' ' << height() << '\n'
             << 255 << '\n';
        if (!fout)
        {
            std::cerr << "ERROR: Could not write PPM header to file." << std::endl;
            return false;
        }

        std::vector<std::uint8_t> pixels(width() * height() * 3, 0);
        for (std::size_t i = 0; i < _data.size(); ++i)
        {
            pixels[i * 3 + 0] = (std::uint8_t)(_data[i].r * 255);
            pixels[i * 3 + 1] = (std::uint8_t)(_data[i].g * 255);
            pixels[i * 3 + 2] = (std::uint8_t)(_data[i].b * 255);
        }
        
        fout.write((char *)&pixels[0], pixels.size() * sizeof(std::uint8_t));
        if (!fout)
        {
            std::cerr << "ERROR: Could not write image data to file." << std::endl;
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

    int _width;
    int _height;
    std::vector<Pixel> _data;

};


#endif  // #indef FILE_IMAGE_H_INCLUDED


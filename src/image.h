// image.h
// Andrew S. Ng
// Started: 2020-10-23
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Header for Image class

#ifndef FILE_IMAGE_H_INCLUDED
#define FILE_IMAGE_H_INCLUDED

#include <fstream>  // For std::ifstream/ofstream
#include <string>   // For std::string
#include <vector>   // For std::vector


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
        Pixel & px = (*this)(x, y);
        px.r = r;
        px.g = g;
        px.b = b;
    }

    void setColor(int x, int y, Image::Pixel px)
    {
        (*this)(x, y) = px;
    }

    Pixel getColor(int x, int y) const
    {
        return (*this)(x, y);
    }

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    bool readPPM(std::string filename);

    bool writePPM(std::string filename) const;

public:

    Pixel & operator()(int x, int y)
    {
        return _data[x + (y * width())];
    }
    const Pixel & operator()(int x, int y) const
    {
        return _data[x + (y * width())];
    }

private:

    bool getString(std::ifstream & ifs, std::string & str) const;

    bool getInt(std::ifstream & ifs, int & num) const;

private:

    int _width;
    int _height;
    std::vector<Pixel> _data;

};


#endif  // #indef FILE_IMAGE_H_INCLUDED


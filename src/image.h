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


// class Image
// Stores an image using floating-point RGB triplets.
class Image
{

// ---- Image: basic pixel struct ----
public:

    struct Pixel
    {
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
    };

// ---- Image: ctors, dctor, move ops ----
public:

    // Default ctor
    // Creates 800x600 image of black pixels.
    Image()
        :_width(800),_height(600),
         _data(width() * height(), Pixel())
    {}

    // Ctor from width/height
    // Creates image of black pixels with given dimension.
    Image(int width, int height)
        :_width(width),_height(height),
         _data(width * height, Pixel())
    {}

    // Dctor, move operations all defaulted.
    ~Image() = default;
    Image(const Image & other) = default;
    Image & operator=(const Image & other) = default;
    Image(Image && other) = default;
    Image & operator=(Image && other) = default;

// ---- Image: general public functions ----
public:

    // setColor
    // Sets color at coords. to given rgb values.
    void setColor(int x, int y, float r, float g, float b)
    {
        Pixel & px = (*this)(x, y);
        px.r = r;
        px.g = g;
        px.b = b;
    }

    // setColor
    // Sets color at coords. to given Pixel.
    void setColor(int x, int y, Pixel px)
    {
        (*this)(x, y) = px;
    }

    // getColor
    // Returns Pixel at given coords.
    Pixel getColor(int x, int y) const
    {
        return (*this)(x, y);
    }

    // width
    // Returns horizontal width of image.
    int width() const
    {
        return _width;
    }

    // height
    // Returns vertical height of image.
    int height() const
    {
        return _height;
    }

    // readPPM
    // Reads image data from given PPM filename, returns true on success.
    bool readPPM(std::string filename);

    // writePPM
    // Writes image data to given PPM filename, returns true on success.
    bool writePPM(std::string filename) const;

// ---- Image: general public operators ----
public:

    // op() (non-const & const)
    // Returns reference to Pixel at given coords.
    Pixel & operator()(int x, int y)
    {
        return _data[x + (y * width())];
    }
    const Pixel & operator()(int x, int y) const
    {
        return _data[x + (y * width())];
    }

// ---- Image: general private functions ----
private:

    // getString
    // Reads in string from a given file stream.
    bool getString(std::ifstream & ifs, std::string & str) const;

    // getInt
    // Reads in int from a given file stream.
    bool getInt(std::ifstream & ifs, int & num) const;

// ---- Image: private data members ----
private:

    int _width;                // Width of image
    int _height;               // Height of image
    std::vector<Pixel> _data;  // Buffer holding image data
                               // stored as floats in RGBRGB order

};  // End class Image


#endif  // #indef FILE_IMAGE_H_INCLUDED


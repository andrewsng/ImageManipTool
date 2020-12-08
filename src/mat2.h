// mat2.h
// Andrew S. Ng
// Started: 2020-11-12
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Header for Mat2 class
// No associated source file

#ifndef FILE_MAT2_H_INCLUDED
#define FILE_MAT2_H_INCLUDED

#include "vec2.h"  // For class Vec2

#include <cstddef>           // For std::size_t
#include <initializer_list>  // For std::initializer_list


class Mat2
{

public:

    using value_type = float;

    using size_type = std::size_t;

    using list_type = std::initializer_list<std::initializer_list<value_type>>;

public:

    Mat2() : _data{ { 0.0f, 0.0f },
                    { 0.0f, 0.0f } }
    {};

    Mat2(list_type list)
    {
        int i = 0, j = 0;
        for (auto & row : list)
        {
            for (auto & elem : row)
            {
                _data[i][j] = elem;
                ++j;
            }
            j = 0;
            ++i;
        }
    }

    ~Mat2() = default;
    Mat2(const Mat2 & other) = default;
    Mat2 & operator=(const Mat2 & other) = default;
    Mat2(Mat2 && other) = default;
    Mat2 & operator=(Mat2 && other) = default;

public:

    value_type * operator[](size_type index)
    {
        return _data[index];
    }
    const value_type * operator[](size_type index) const
    {
        return _data[index];
    }

    Mat2 inverse() const
    {
        const Mat2 & mat = *this;
        value_type k = 1.0f / (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
        return Mat2{ {  k * mat[1][1], -k * mat[0][1] },
                     { -k * mat[1][0],  k * mat[0][0] } };
    }

    Vec2 operator*(const Vec2 & vec) const
    {
        const Mat2 & mat = *this;
        return Vec2(mat[0][0] * vec.x + mat[0][1] * vec.y,
                    mat[1][0] * vec.x + mat[1][1] * vec.y);
    }

private:

    value_type _data[2][2];

};


#endif  // #ifndef FILE_MAT2_H_INCLUDED
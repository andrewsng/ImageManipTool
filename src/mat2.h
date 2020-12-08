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


// class Mat2
// 2x2 Matrix of floats
class Mat2
{

// ---- Mat2: public member types ----
public:

    // Matrix values are floats
    using value_type = float;

    // size_t for indices
    using size_type = std::size_t;

    // 2D initializer list for convenient construction
    using list_type = std::initializer_list<std::initializer_list<value_type>>;

// ---- Mat2: ctors, dctor, move ops ----
public:

    // Default ctor
    // Creates matrix of all zeros.
    Mat2() : _data{ { 0.0f, 0.0f },
                    { 0.0f, 0.0f } }
    {};

    // Ctor from list
    // Sets values based on 2D init list.
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

    // Dctor, move operations all defaulted.
    ~Mat2() = default;
    Mat2(const Mat2 & other) = default;
    Mat2 & operator=(const Mat2 & other) = default;
    Mat2(Mat2 && other) = default;
    Mat2 & operator=(Mat2 && other) = default;

// ---- Mat2: general public functions ----
public:

    // inverse
    // returns the inverse of this matrix
    Mat2 inverse() const
    {
        const Mat2 & mat = *this;
        value_type k = 1.0f / (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]);
        return Mat2{ {  k * mat[1][1], -k * mat[0][1] },
                     { -k * mat[1][0],  k * mat[0][0] } };
    }

// ---- Mat2: general public operators ----
public:

    // op[] (non-const & const)
    // Returns pointer to row based on index.
    value_type * operator[](size_type index)
    {
        return _data[index];
    }
    const value_type * operator[](size_type index) const
    {
        return _data[index];
    }

    // op*(Mat2, Vec2)
    // Matrix-vector multiplication using Vec2 class.
    // Implemented as member function to restrict mult. order.
    Vec2 operator*(const Vec2 & vec) const
    {
        const Mat2 & mat = *this;
        return Vec2(mat[0][0] * vec.x + mat[0][1] * vec.y,
                    mat[1][0] * vec.x + mat[1][1] * vec.y);
    }

// ---- Mat2: private data members ----
private:

    // 2x2 2D array of value type.
    value_type _data[2][2];

};  // End class Mat2


#endif  // #ifndef FILE_MAT2_H_INCLUDED
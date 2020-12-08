// vec2.h
// Andrew S. Ng
// Started: 2020-11-12
// Updated: 2020-12-07
//
// For CS 301 Fall 2020
// Header for Vec2 class
// No associated source file

#ifndef FILE_VEC2_H_INCLUDED
#define FILE_VEC2_H_INCLUDED

#include <cstddef>  // For std::size_t


// struct Vec2
// Vector of 2 floats
struct Vec2
{

// ---- Vec2: public member types ----
public:

    // Vector values are floats.
    using value_type = float;
    
    // size_t for indices.
    using size_type = std::size_t;

// ---- Vec2: ctors, dctor, move ops ----
public:

    // Default ctor
    // Initialize both values to zero.
    Vec2() : x(0.0f), y(0.0f)
    {};

    // Ctor from x/y
    // Initialize using given x and y.
    Vec2(value_type initX, value_type initY) : x(initX), y(initY)
    {};

    // Dctor, move operations all defaulted.
    ~Vec2() = default;
    Vec2(const Vec2 & other) = default;
    Vec2 & operator=(const Vec2 & other) = default;
    Vec2(Vec2 && other) = default;
    Vec2 & operator=(Vec2 && other) = default;

// ---- Vec2: public data members ----
public:

    value_type x;
    value_type y;

};  // End struct Vec2


#endif  // #ifndef FILE_VEC2_H_INCLUDED
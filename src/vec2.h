#ifndef FILE_VEC2_H_INCLUDED
#define FILE_VEC2_H_INCLUDED

#include <cstddef>  // For std::size_t


struct Vec2
{

public:

    using value_type = float;
    
    using size_type = std::size_t;

public:

    Vec2() : x(0.0f), y(0.0f)
    {};

    Vec2(value_type initX, value_type initY) : x(initX), y(initY)
    {};

    ~Vec2() = default;
    Vec2(const Vec2 & other) = default;
    Vec2 & operator=(const Vec2 & other) = default;
    Vec2(Vec2 && other) = default;
    Vec2 & operator=(Vec2 && other) = default;

public:

    value_type x;
    value_type y;

};


#endif  // #ifndef FILE_VEC2_H_INCLUDED
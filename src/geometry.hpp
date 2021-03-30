#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <functional>

template<size_t Size, typename T>
class Point {
    
static_assert(Size >= 1);
friend std::ostream& operator<<(std::ostream& stream, const Point& point) {
    return stream << point.to_string();
}
public:
    std::array<T, Size> values {};

    Point() {}
    Point(const Point<Size, T>& other) : values {other.values} {}
    Point(T x, T y, T z)
        : values {x, y, z}
    {
        static_assert(Size == 3);
    }
    Point(T x, T y)
            : values {x, y}
    {
        static_assert(Size == 2);
    }

    T& x() { return values[0]; }
    T x() const { return values[0]; }
    T& y() {
        static_assert(Size >= 2);
        return values[1];
    }
    T y() const {
        static_assert(Size >= 2);
        return values[1];
    }
    T& z() {
        static_assert(Size >= 3);
        return values[2];
    }
    T z() const {
        static_assert(Size >= 3);
        return values[2];
    }
    template<int pos>
    [[nodiscard]] T get() const {
        static_assert(Size > pos);
        return values[pos];
    }
    template<int pos>
    [[nodiscard]] T& get() {
        static_assert(Size > pos);
        return values[pos];
    }

    Point<Size, T>& operator+=(const Point<Size, T>& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::plus<T>());
        return *this;
    }

    Point<Size, T>& operator-=(const Point<Size, T>& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::minus<T>());
        return *this;
    }
    Point<Size, T>& operator*=(const Point<Size, T>& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::multiplies<T>());
        return *this;
    }

    Point<Size, T>& operator*=(const T scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](T x){return x*scalar;});
        return *this;
    }

    Point<Size, T> operator+(const Point<Size, T>& other) const
    {
        Point<Size, T> result = *this;
        result += other;
        return result;
    }

    Point<Size, T> operator-(const Point<Size, T>& other) const
    {
        Point<Size, T> result = *this;
        result -= other;
        return result;
    }

    Point<Size, T> operator*(const T scalar) const
    {
        Point<Size, T> result = *this;
        result *= scalar;
        return result;
    }
    Point<Size, T> operator*(const Point<Size, T>& other) const
    {
        Point<Size, T> result = *this;
        result *= other;
        return result;
    }

    Point<Size, T> operator-() const {
        Point<Size, T> result;
        return result - *this;
    }

    T length() const {
        const T l = std::accumulate(values.begin(), values.end(), static_cast<T>(0), [](T x, T y){ return x + y*y;});
        return std::sqrt(l);
    }

    T distance_to(const Point<Size, T>& other) const { return (*this - other).length(); }

    Point<Size, T>& normalize(const T target_len = 1.0f)
    {
        const T current_len = length();
        if (current_len == 0) throw std::logic_error("cannot normalize vector of length 0");

        *this *= (target_len / current_len);
        return *this;
    }

    Point<Size, T>& cap_length(const T max_len)
    {
        assert(max_len > 0);

        const T current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }
        return *this;
    }

    std::string to_string() const {
        using namespace std::string_literals;
        return "[" + std::accumulate(std::next(values.begin()), values.end(), std::to_string(values[0]),
                               [](const std::string& s, const T& x){return s + ", "s + std::to_string(x);}) + "]";
    }
};
// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
template<typename T>
inline Point<2, T> project_2D(const Point<3, T>& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}



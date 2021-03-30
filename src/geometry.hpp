#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <functional>

struct Point2D
{
    float values[2] {};

    Point2D() {}
    ~Point2D() = default;
    Point2D(float x, float y) : values { x, y } {}
    Point2D(const Point2D& o) : values {o.values[0], o.values[1]} {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    Point2D& operator+=(const Point2D& other)
    {
        x() += other.x();
        y() += other.y();
        return *this;
    }

    Point2D& operator*=(const Point2D& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point2D& operator*=(const float scalar)
    {
        x() *= scalar;
        y() *= scalar;
        return *this;
    }

    Point2D operator+(const Point2D& other) const
    {
        Point2D result = *this;
        result += other;
        return result;
    }

    Point2D operator*(const Point2D& other) const
    {
        Point2D result = *this;
        result *= other;
        return result;
    }

    Point2D operator*(const float scalar) const
    {
        Point2D result = *this;
        result *= scalar;
        return result;
    }
};

struct Point3D
{
    std::array<float, 3> values {};

    Point3D() {}
    Point3D(float x, float y, float z) : values { x, y, z } {}
    Point3D(const Point3D& other) : values {other.values} {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    float& z() { return values[2]; }
    float z() const { return values[2]; }

    Point3D& operator+=(const Point3D& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::plus<>());
        return *this;
    }

    Point3D& operator-=(const Point3D& other)
    {
        std::transform(values.cbegin(), values.cend(), other.values.cbegin(), values.begin(),
                       std::minus<>());
        return *this;
    }

    Point3D& operator*=(const float scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](float x){return x*scalar;});
        return *this;
    }

    Point3D operator+(const Point3D& other) const
    {
        Point3D result = *this;
        result += other;
        return result;
    }

    Point3D operator-(const Point3D& other) const
    {
        Point3D result = *this;
        result -= other;
        return result;
    }

    Point3D operator*(const float scalar) const
    {
        Point3D result = *this;
        result *= scalar;
        return result;
    }

    Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }

    float length() const {
        const float l = std::accumulate(values.begin(), values.end(), 0.f, [](float x, float y){ return x + y*y;});
        return std::sqrt(l);
    }

    float distance_to(const Point3D& other) const { return (*this - other).length(); }

    Point3D& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point3D& cap_length(const float max_len)
    {
        assert(max_len > 0);

        const float current_len = length();
        if (current_len > max_len)
        {
            *this *= (max_len / current_len);
        }

        return *this;
    }

    std::string to_string() const {
        return "[" + std::to_string(values[0]) + ", " + std::to_string(values[1]) +
        ", " + std::to_string(values[2]) + "]";
    }
};

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

template<size_t Size, typename T>
class Point {
    static_assert(Size >= 1);
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

    Point<Size, T> operator-() const {
        Point<Size, T> result { *this };
        std::transform(values.cbegin(), values.cend(), values.begin(), [](const T& v){return -v;});
        return result;
    }

    T length() const {
        const T l = std::accumulate(values.begin(), values.end(), static_cast<T>(0), [](T x, T y){ return x + y*y;});
        return std::sqrt(l);
    }

    // Same as length
    T distance_to(const Point<Size, T>& other) const { return (*this - other).length(); }

    Point<Size, T>& normalize(const T target_len = 1.0f)
    {
        const T current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

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
    friend std::ostream& operator<<(std::ostream& stream, const Point& point) {
        return stream << point.to_string();
    }
};





#pragma once

#include "geometry.hpp"

// runway positions relative to the airport position
struct Runway
{
    const Point<3, float> start, end;

    Runway(const Point<3, float>& start_, const float length = 1) :
        start { start_ }, end { start_ + Point<3, float> { length, 0.f, 0.f } }
    {}
};

#pragma once

#include "geometry.hpp"

#include <deque>

enum WaypointType
{
    wp_air,
    wp_ground,
    wp_terminal
};

class Waypoint : public Point3D
{
friend std::ostream& operator<<(std::ostream& s, const Waypoint& w) {
    s << "WP( [" << w.values[0] << ", " << w.values[1] << ", " << w.values[2] << "], " << w.type << ")";
    return s;
}
public:
    WaypointType type;

    Waypoint(const Point3D& position, const WaypointType type_ = wp_air) :
        Point3D { position }, type { type_ }
    {}
    Waypoint(const Waypoint& other) = default;

    bool is_on_ground() const { return type != wp_air; }
    bool is_at_terminal() const { return type == wp_terminal; }
    Waypoint& operator=(const Waypoint& other) {
        if (this != &other) {
            values = other.values;
            type = other.type;
        }
        return *this;
    }
};

using WaypointQueue = std::deque<Waypoint>;
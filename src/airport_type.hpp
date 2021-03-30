#pragma once

#include "geometry.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "waypoint.hpp"

#include <initializer_list>
#include <vector>

class AirportType
{
private:
    const Point<3, float> crossing_pos;
    const Point<3, float> gateway_pos;
    const std::vector<Point<3, float>> terminal_pos;
    const std::vector<Runway> runways;

public:
    AirportType& operator=(const AirportType& other) = delete;
    AirportType(const AirportType&) = delete;
    ~AirportType() = default;
    AirportType(const Point<3, float>& crossing_pos_, const Point<3, float>& gateway_pos_,
                std::initializer_list<Point<3, float>> terminal_pos_, std::initializer_list<Runway> runways_) :
        crossing_pos { crossing_pos_ },
        gateway_pos { gateway_pos_ },
        terminal_pos { terminal_pos_ },
        runways { runways_ }
    {}

    [[nodiscard]] std::vector<Terminal> create_terminals() const
    {
        return std::vector<Terminal> { terminal_pos.begin(), terminal_pos.end() };
    }

    [[nodiscard]] WaypointQueue air_to_terminal(const Point<3, float>& offset, const size_t runway_num,
                                  const size_t terminal_num) const
    {
        assert(runway_num < runways.size());
        assert(terminal_num < terminal_pos.size());
        const Runway& runway = runways.at(runway_num);

        const auto runway_middle_pos = (runway.start + runway.end) * 0.5f;
        const auto runway_length     = (runway.end - runway.start) * 0.5f;

        const Waypoint before_in_air { offset + runway.start - runway_length + Point<3, float> { 0.f, 0.f, .7f },
                                       wp_air };
        const Waypoint runway_middle { offset + runway_middle_pos, wp_ground };
        const Waypoint runway_end { offset + runway.end, wp_ground };
        const Waypoint crossing { offset + crossing_pos, wp_ground };

        WaypointQueue result { before_in_air, runway_middle, runway_end, crossing };

        if (terminal_num != 0)
        {
            result.emplace_back(gateway_pos, wp_ground);
        }

        result.emplace_back(terminal_pos.at(terminal_num), wp_terminal);
        return result;
    }

    [[nodiscard]] WaypointQueue terminal_to_air(const Point<3, float>& offset, const size_t runway_num,
                                  const size_t terminal_num) const
    {
        assert(runway_num < runways.size());
        assert(terminal_num < terminal_pos.size());
        const Runway& runway = runways.at(runway_num);
        const float angle    = (float)(rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi

        const auto runway_middle_pos = (runway.start + runway.end) * 0.5f;
        const auto runway_length     = (runway.end - runway.start) * 0.5f;

        const Waypoint crossing { offset + crossing_pos, wp_ground };
        const Waypoint runway_start { offset + runway.start, wp_ground };
        const Waypoint runway_middle { offset + runway_middle_pos, wp_ground };
        const Waypoint later_in_air { offset + runway.end + runway_length + Point<3, float> { 0.f, 0.f, .7f }, wp_air };
        const Waypoint randomly_high {
            Point<3, float> { std::sin(angle), std::cos(angle), 0.f } * 6 + Point<3, float> { 0.f, 0.f, 2.f }, wp_air
        };

        WaypointQueue result { crossing, runway_start, runway_middle, later_in_air, randomly_high };

        if (terminal_num != 0)
        {
            result.emplace_front(gateway_pos, wp_ground);
        }

        return result;
    }
};

inline const AirportType one_lane_airport { Point<3, float> { -.1f, -.3f, 0.f },
                                            Point<3, float> { -.6f, .3f, 0.f },
                                            { Point<3, float> { .3f, 0.f, 0.f }, Point<3, float> { -.3f, .3f, 0.f },
                                              Point<3, float> { 0.f, .55f, 0.f } },
                                            { Runway { Point<3, float> { -.5f, -.75f, 0.f } } } };

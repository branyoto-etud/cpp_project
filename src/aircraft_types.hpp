#pragma once

#include "GL/texture.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <array>

struct AircraftType
{
    const float fuel_consumption;
    const float max_ground_speed;
    const float max_air_speed;
    const unsigned max_fuel;
    const float max_accel;
    const GL::Texture2D texture;

    AircraftType(const float max_ground_speed_, const float max_air_speed_, const float max_accel_,
                 const float fuel_consumption_, const unsigned max_fuel_, const MediaPath& sprite,
                 const size_t num_tiles = NUM_AIRCRAFT_TILES) :
        fuel_consumption {fuel_consumption_},
        max_ground_speed { max_ground_speed_ },
        max_air_speed { max_air_speed_ },
        max_fuel { max_fuel_ },
        max_accel { max_accel_ },
        texture { new img::Image { sprite.get_full_path() }, num_tiles }
    {}

    [[nodiscard]] float min_fuel() const {
        //        consumption for 10      seconds
        std::cout << fuel_consumption * 10 * (float)GL::ticks_per_sec << std::endl;
        return fuel_consumption * 10 * (float)GL::ticks_per_sec;
    }
};
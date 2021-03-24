#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include "geometry.hpp"

#include <cassert>

class Terminal : public GL::DynamicObject
{
private:
    double service_progress    = SERVICE_CYCLES;
    const Aircraft* current_aircraft = nullptr;
    const Point3D pos;

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

public:
    explicit Terminal(const Point3D& pos_) : pos { pos_ } {}

    bool in_use() const { return current_aircraft != nullptr; }
    bool is_servicing() const { return service_progress < SERVICE_CYCLES; }
    void assign_craft(const Aircraft& aircraft) { current_aircraft = &aircraft; }

    void start_service(const Aircraft& aircraft)
    {
        assert(aircraft.distance_to(pos) < DISTANCE_THRESHOLD);
        if (!SILENT_TERMINAL)
            std::cout << "now servicing " << aircraft.get_flight_num() << "...\n";
        service_progress = 0;
    }

    void finish_service()
    {
        if (is_servicing()) return;
        if (!SILENT_TERMINAL) std::cout << "done servicing " << current_aircraft->get_flight_num() << '\n';
        current_aircraft = nullptr;
    }

    void move(double alpha) override
    {
        if (in_use() && is_servicing())
        {
            service_progress += alpha;
        }
    }
};
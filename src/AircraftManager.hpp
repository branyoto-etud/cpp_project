#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include "aircraft.hpp"
#include "GL/dynamic_object.hpp"

class AircraftManager : public GL::DynamicObject
{
public:
    AircraftManager();                       // Base Constructor
    ~AircraftManager() override;             // Destructor
    AircraftManager(const AircraftManager&) = delete;
    AircraftManager& operator=(const AircraftManager&) = delete;

    void add_aircraft(std::unique_ptr<Aircraft>);
    void move(double) override;
    unsigned count_aircraft_on_airline(const std::string_view&);
    unsigned get_required_fuel();

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
};

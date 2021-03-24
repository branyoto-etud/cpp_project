#pragma once

#include <ostream>
#include <vector>
#include <memory>

#include "aircraft_types.hpp"
#include <fstream>

class Aircraft;
class Tower;

inline std::array<std::string, 8> airlines { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

class AircraftFactory
{
public:
    AircraftFactory();                       // Base Constructor

    ~AircraftFactory();                      // Destructor
    static AircraftFactory* LoadTypes(const MediaPath&);

    std::unique_ptr<Aircraft> create_aircraft(Tower& tower);
    AircraftFactory(const AircraftFactory&) = delete; // Copy Constructor

    AircraftFactory& operator=(const AircraftFactory&) = delete; // Assignment
private:
    static std::unique_ptr<AircraftType> parse_line(std::string&);
    explicit AircraftFactory(std::ifstream&);

    std::string get_flight_number();

    std::vector<std::unique_ptr<AircraftType>> aircraft_types;
    std::unordered_set<std::string> flight_numbers;
};

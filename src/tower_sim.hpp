#pragma once

#include <string>

class Airport;
class AircraftManager;
struct AircraftType;
class AircraftFactory;

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager* aircraft_manager = nullptr;
    AircraftFactory* aircraft_factory = nullptr;
    std::string data_path;

    void create_random_aircraft();

    void create_keystrokes();
    static void display_help() ;
    void display_airline(unsigned);

    void init_airport();
public:
    ~TowerSimulation();
    TowerSimulation(int argc, char** argv);
    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void launch();
};

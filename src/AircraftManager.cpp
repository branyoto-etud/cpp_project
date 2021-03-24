#include "AircraftManager.hpp"

#include <numeric>
#include <algorithm>

AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}

AircraftManager::~AircraftManager() = default;

void AircraftManager::move(const double alpha)
{
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](const std::unique_ptr<Aircraft>& a, const std::unique_ptr<Aircraft>& b){return *a < *b;});
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                  [alpha](const std::unique_ptr<Aircraft>& a){return a->move(alpha);}), aircrafts.end());
}
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    aircrafts.emplace_back(std::move(aircraft));
}

unsigned AircraftManager::count_aircraft_on_airline(const std::string_view& line)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(),
        [line](const std::unique_ptr<Aircraft>& a){return (a->get_flight_num().rfind(line, 0) == 0);});
}

unsigned AircraftManager::get_required_fuel() {
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
           [](unsigned x, const std::unique_ptr<Aircraft>& a){
               return a->is_low_on_fuel() && a->is_circling() ? a->get_missing_fuel() + x : x;
           });
}

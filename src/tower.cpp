#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <cassert>

WaypointQueue Tower::get_circle()
{
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft)
{
    if (aircraft.is_at_terminal)                                                // If the aircraft is at terminal
    {
        const auto it = reserved_terminals.find(&aircraft);                     // Find the aircraft
        assert(it != reserved_terminals.end());                                 // Ensure the aircraft is found
        const auto terminal_num = it->second;                                   // Get the terminal number
        Terminal& terminal      = airport.get_terminal(terminal_num);           // Get the terminal
        if (terminal.is_servicing())                                            // If not done servicing
            return {};
        terminal.finish_service();                                              // Remove the aircraft from terminal
        reserved_terminals.erase(it);                                           // Remove the terminal from reserved
        aircraft.is_at_terminal = false;
        return airport.start_path(terminal_num);                                // Create a path to let the aircraft fly
    }
    if (aircraft.distance_to(airport.pos) >= 5) return get_circle();            // If the aircraft is far -> circle
    const auto vp = airport.reserve_terminal(aircraft);                         // Try to reserve a terminal
    if (vp.first.empty()) return get_circle();                                  // If no terminal left -> circle
    reserved_terminals.emplace(&aircraft, vp.second);                           // Otherwise -> reserve the terminal found
    return vp.first;                                                            // Return the path to the terminal
}

void Tower::arrived_at_terminal(const Aircraft& aircraft)
{
    const auto it = reserved_terminals.find(&aircraft);
    assert(it != reserved_terminals.end());
    airport.get_terminal(it->second).start_service(aircraft);
}


WaypointQueue Tower::reserve_terminal(Aircraft& aircraft)
{
    if (aircraft.has_terminal()) return {};                           // If the aircraft already have a terminal
    if (aircraft.distance_to(airport.pos) >= 5) return {};            // If the aircraft is far -> cannot give him a terminal
    const auto vp = airport.reserve_terminal(aircraft);            // Try to reserve a terminal
    if (vp.first.empty()) return {};                                  // If no terminal left -> empty
    reserved_terminals.emplace(&aircraft, vp.second);                 // Otherwise -> reserve the terminal found
    return vp.first;                                                  // Return the path to the terminal
}

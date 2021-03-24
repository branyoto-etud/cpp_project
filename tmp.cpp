bool Aircraft::move(double alpha)
{
    if (fuel <= 0) return true;
    if (!is_on_ground()) fuel -= alpha;
    if (waypoints.empty())
        waypoints = control.get_instructions(*this);
    bool is_lifting = false;
    if (!is_at_terminal)
    {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed * (float)alpha;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                is_lifting = operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash { flight_number + " crashed into the ground"s };
            }
        }
        else
        {
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }
    return is_lifting;
}



bool Aircraft::move(double alpha)
{
    if (fuel < 0) {                                                         // Crash when no fuel
        std::cout << flight_number << " run out of fuel!" << std::endl;
        return true;
    }
    if (!is_on_ground()) fuel-=alpha;                                       // Decrease fuel
    if (waypoints.empty()) waypoints = control.get_instructions(*this);     // Get a path if empty
//    if (is_circling()) {
//        auto wp = control.reserve_terminal(*this);
//        if (!wp.empty()) waypoints = control.reserve_terminal(*this);
//    }
    if (is_at_terminal) return false;                                       // If aircraft is at terminal cannot move

    turn_to_waypoint();                                                     // Rotate the aircraft
    pos += speed * (float)alpha;                                            // Move it

    if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)      // Remove waypoint if close enough
    {
        if (waypoints.front().is_at_terminal()) arrive_at_terminal();                   // start servicing
        else if (operate_landing_gear()) return true;                                   // if landing gear retract -> aircraft need to be destroyed
        waypoints.pop_front();                                                          // Remove waypoint
    }

    if (is_on_ground() && !landing_gear_deployed)                                       // Incorrect state caused by speed
        throw AircraftCrash { flight_number + " crashed into the ground" };

    if (speed.length() < SPEED_THRESHOLD)                                               // Sink if to slow
        pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed.length());

    GL::Displayable::z = pos.x() + pos.y();                                             // Update z value
    return false;
}



bool Aircraft::move(double alpha)
{
    if (fuel < 0) {                                                         // Crash when no fuel
        std::cout << flight_number << " run out of fuel!" << std::endl;
        return true;
    }
    if (!is_on_ground()) fuel-=alpha;                                       // Decrease fuel
    if (waypoints.empty()) waypoints = control.get_instructions(*this);     // Get a path if empty
    if (is_circling()) {
        auto wp = control.reserve_terminal(*this);
        if (!wp.empty()) {
            waypoints = wp;
        }
    }
    if (is_at_terminal) return false;                                       // If aircraft is at terminal cannot move

    turn_to_waypoint();                                                     // Rotate the aircraft
    pos += speed * (float)alpha;                                            // Move it

    if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)      // Remove waypoint if close enough
    {
        if (waypoints.front().is_at_terminal()) arrive_at_terminal();                   // start servicing
        else if (operate_landing_gear()) return true;                                   // if landing gear retract -> aircraft need to be destroyed
        waypoints.pop_front();                                                          // Remove waypoint
    }

    if (is_on_ground() && !landing_gear_deployed)                                       // Incorrect state caused by speed
        throw AircraftCrash { flight_number + " crashed into the ground" };

    if (speed.length() < SPEED_THRESHOLD)                                               // Sink if to slow
        pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed.length());

    GL::Displayable::z = pos.x() + pos.y();                                             // Update z value
    return false;
}
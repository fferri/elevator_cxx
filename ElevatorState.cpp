#include "ElevatorState.h"
#include "PreconditionException.h"

#include <sstream>

ElevatorState::ElevatorState(int at_, const set<int>& lights_)
    : at(at_), lights(lights_)
{
}

ElevatorState::~ElevatorState()
{
}

string ElevatorState::str() const
{
    std::stringstream ss;
    ss << "ElevatorState(" << at << ", {";
    const char *sep = "";
    for(const int& i : lights)
    {
        ss << sep << i;
        sep = ", ";
    }
    ss << "})";
    return ss.str();
}

ElevatorStatePtr ElevatorState::up() const
{
    if(at >= 6)
        throw PreconditionException("maximum floor 6");
    return ElevatorState::make_shared(at + 1, lights);
}

ElevatorStatePtr ElevatorState::down() const
{
    if(at <= 0)
        throw PreconditionException("minimum floor 0");
    return ElevatorState::make_shared(at - 1, lights);
}

ElevatorStatePtr ElevatorState::turnOff() const
{
    if(lights.find(at) == lights.end())
        throw PreconditionException("current floor is no lit");
    set<int> lights1(lights);
    lights1.erase(at);
    return ElevatorState::make_shared(at, lights1);
}

ostream& operator<<(ostream& os, const ElevatorState& s)
{
    os << s.str();
    return os;
}


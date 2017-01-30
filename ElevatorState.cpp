#include "ElevatorState.h"
#include "PreconditionException.h"

#include <sstream>

ElevatorState::ElevatorState(int at_, const std::set<int> &lights_)
    : at(at_), lights(lights_)
{
}

ElevatorState::~ElevatorState()
{
}

std::string ElevatorState::str() const
{
    std::stringstream ss;
    ss << "ElevatorState(" << at << ", {";
    const char *sep = "";
    for(const int &i : lights)
    {
        ss << sep << i;
        sep = ", ";
    }
    ss << "})";
    return ss.str();
}

ElevatorState::Ptr ElevatorState::up() const
{
    if(at >= 6)
        throw PreconditionException("maximum floor 6");
    return std::make_shared<ElevatorState>(at + 1, lights);
}

ElevatorState::Ptr ElevatorState::down() const
{
    if(at <= 0)
        throw PreconditionException("minimum floor 0");
    return std::make_shared<ElevatorState>(at - 1, lights);
}

ElevatorState::Ptr ElevatorState::turnOff() const
{
    if(lights.find(at) == lights.end())
        throw PreconditionException("current floor is no lit");
    std::set<int> lights1(lights);
    lights1.erase(at);
    return std::make_shared<ElevatorState>(at, lights1);
}

std::ostream& operator<<(std::ostream &os, const ElevatorState &s)
{
    os << s.str();
    return os;
}


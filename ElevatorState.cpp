#include "ElevatorState.h"
#include "PreconditionException.h"

#include <sstream>

ElevatorState::ElevatorState(int at_, set<int>& lights_)
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
    ElevatorStatePtr s = make_shared<ElevatorState>(*this);
    s->at++;
    return s;
}

ElevatorStatePtr ElevatorState::down() const
{
    if(at <= 0)
        throw PreconditionException("minimum floor 0");
    ElevatorStatePtr s = make_shared<ElevatorState>(*this);
    s->at--;
    return s;
}

ElevatorStatePtr ElevatorState::turnOff() const
{
    if(lights.find(at) == lights.end())
        throw PreconditionException("current floor is no lit");
    ElevatorStatePtr s = make_shared<ElevatorState>(*this);
    s->lights.erase(at);
    return s;
}

ostream& operator<<(ostream& os, const ElevatorState& s)
{
    os << s.str();
    return os;
}


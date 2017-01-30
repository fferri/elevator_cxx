#ifndef ELEVATORSTATE_H_INCLUDED
#define ELEVATORSTATE_H_INCLUDED

#include "Util.h"

#include <string>
#include <set>
#include <iostream>
#include <memory>

class ElevatorState
{
public:
    const int at;
    const std::set<int> lights;
public:
    using Ptr = std::shared_ptr<ElevatorState>;
    ElevatorState() = delete;
    ElevatorState(int at_, const std::set<int> &lights_);
    ElevatorState(const ElevatorState &s) = default;
    virtual ~ElevatorState();
    ElevatorState& operator=(const ElevatorState &s) = delete;
    std::string str() const;
    ElevatorState::Ptr up() const;
    ElevatorState::Ptr down() const;
    ElevatorState::Ptr turnOff() const;
};

std::ostream& operator<<(std::ostream &os, const ElevatorState &s);

#endif // ELEVATORSTATE_H_INCLUDED


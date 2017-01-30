#ifndef ELEVATORSTATE_H_INCLUDED
#define ELEVATORSTATE_H_INCLUDED

#include "Util.h"

#include <string>
#include <set>
#include <iostream>
#include <memory>

using std::string;
using std::set;
using std::ostream;
using std::shared_ptr;
using std::make_shared;

class ElevatorState : public add_make_shared<ElevatorState>
{
public:
    const int at;
    const set<int> lights;
public:
    ElevatorState() = delete;
    ElevatorState(int at_, const set<int> &lights_);
    ElevatorState(const ElevatorState &s) = default;
    virtual ~ElevatorState();
    ElevatorState& operator=(const ElevatorState &s) = delete;
    string str() const;
    Ptr up() const;
    Ptr down() const;
    Ptr turnOff() const;
};

typedef ElevatorState::Ptr ElevatorStatePtr;

ostream& operator<<(ostream &os, const ElevatorState &s);

#endif // ELEVATORSTATE_H_INCLUDED


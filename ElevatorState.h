#ifndef ELEVATORSTATE_H_INCLUDED
#define ELEVATORSTATE_H_INCLUDED

#include <string>
#include <set>
#include <iostream>
#include <memory>

using std::string;
using std::set;
using std::ostream;
using std::shared_ptr;
using std::make_shared;

class ElevatorState
{
public:
    int at;
    set<int> lights;
public:
    typedef shared_ptr<ElevatorState> Ptr;

    ElevatorState() = delete;
    ElevatorState(int at_, set<int>& lights_);
    ElevatorState(const ElevatorState& s) = default;
    virtual ~ElevatorState();
    ElevatorState & operator=(const ElevatorState& s) = delete;
    string str() const;
    Ptr up() const;
    Ptr down() const;
    Ptr turnOff() const;
};

typedef ElevatorState::Ptr ElevatorStatePtr;

ostream& operator<<(ostream& os, const ElevatorState& s);

#endif // ELEVATORSTATE_H_INCLUDED


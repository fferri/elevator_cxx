#ifndef GOLOG_PROGRAMSTATE_H_INCLUDED
#define GOLOG_PROGRAMSTATE_H_INCLUDED

#include <memory>
#include <vector>
#include <string>

#include "ElevatorState.h"

class Program;

// TODO: use templates for State type
using State = ElevatorState;

struct ProgramState {
    using Ptr = std::shared_ptr<ProgramState>;
    
    Ptr parent;

    const std::shared_ptr<Program> program;
    const std::shared_ptr<State> state;
    const std::shared_ptr<Program> action;
    
    ProgramState(const std::shared_ptr<Program> program_, const std::shared_ptr<State> state_, const ProgramState::Ptr parent_ = nullptr, const std::shared_ptr<Program> action_ = nullptr);
    
    ProgramState(const ProgramState &ps);
    
    void trans(std::vector<ProgramState> &result);
    
    bool isFinal();

    std::string str() const;
};

ostream& operator<<(ostream &os, const ProgramState &ps);

using ProgramStateVector = std::vector<ProgramState>;

#endif // GOLOG_PROGRAMSTATE_H_INCLUDED


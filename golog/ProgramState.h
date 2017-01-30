#ifndef GOLOG_PROGRAMSTATE_H_INCLUDED
#define GOLOG_PROGRAMSTATE_H_INCLUDED

#include <memory>
#include <vector>
#include <string>
#include <sstream>

template<typename S>
class Program;

template<typename S>
struct ProgramState {
    using Ptr = std::shared_ptr<ProgramState>;
    
    ProgramState::Ptr parent;

    const std::shared_ptr<Program<S>> program;
    const std::shared_ptr<S> state;
    const std::shared_ptr<Program<S>> action;
    
    ProgramState(const std::shared_ptr<Program<S>> program_, const std::shared_ptr<S> state_, const ProgramState<S>::Ptr parent_ = nullptr, const std::shared_ptr<Program<S>> action_ = nullptr)
    : program(program_), state(state_), parent(parent_), action(action_)
    {
    }
    
    ProgramState(const ProgramState<S> &ps)
    : program(ps.program), state(ps.state), action(ps.action)
    {
    }
    
    void trans(std::vector<ProgramState<S>> &result)
    {
        program->trans(state, result);
    }
    
    bool isFinal()
    {
        return program->isFinal(state);
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << "ProgramState(" << program->str() << ", " << state->str();
        if(action) ss << ", ACTION=" << *action;
        ss << ")";
        return ss.str();
    }
};

template<typename S>
ostream& operator<<(ostream &os, const ProgramState<S> &ps)
{
    os << ps.str();
    return os;
}

template<typename S>
using ProgramStateVector = std::vector<ProgramState<S>>;

#endif // GOLOG_PROGRAMSTATE_H_INCLUDED


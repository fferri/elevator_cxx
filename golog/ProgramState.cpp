#include "ProgramState.h"
#include "Program.h"

#include <sstream>

ProgramState::ProgramState(const std::shared_ptr<Program> program_, const std::shared_ptr<State> state_, const ProgramState::Ptr parent_, const std::shared_ptr<Program> action_)
: program(program_), state(state_), parent(parent_), action(action_)
{
}

ProgramState::ProgramState(const ProgramState &ps)
: program(ps.program), state(ps.state), action(ps.action)
{
}

void ProgramState::trans(std::vector<ProgramState> &result)
{
    program->trans(state, result);
}

bool ProgramState::isFinal()
{
    return program->isFinal(state);
}

std::string ProgramState::str() const
{
    std::stringstream ss;
    ss << "ProgramState(" << program->str() << ", " << state->str();
    if(action) ss << ", ACTION=" << *action;
    ss << ")";
    return ss.str();
}

ostream& operator<<(ostream &os, const ProgramState &ps)
{
    os << ps.str();
    return os;
}

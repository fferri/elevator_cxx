#include "Exec.h"
#include "Empty.h"

Exec::Exec(Action action_)
: action(action_)
{
}

void Exec::trans(State::Ptr state, ProgramStateVector &result)
{
    try
    {
        result.push_back(ProgramState(empty(), action(state), nullptr, shared_from_this()));
    }
    catch(PreconditionException &ex)
    {
    }
}

bool Exec::isFinal(State::Ptr state)
{
    return false;
}

string Exec::str() const
{
    return "Exec(...)";
}

Program::Ptr exec(Action a) {
    return Exec::make_shared(a);
}

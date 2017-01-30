#include "Star.h"
#include "Empty.h"
#include "Sequence.h"

Star::Star(Program::Ptr p_)
: p(p_)
{
}

void Star::trans(State::Ptr state, ProgramStateVector &result)
{
    ProgramStateVector result1;
    p->trans(state, result1);
    for(ProgramState &ps : result1)
        result.push_back(ProgramState(seq(ps.program, shared_from_this()), ps.state, nullptr, ps.action));
    result.push_back(ProgramState(empty(), state));
}

bool Star::isFinal(State::Ptr state)
{
    return false;
}

string Star::str() const
{
    string s = "Star(";
    s += p->str();
    s += ")";
    return s;
}

Program::Ptr star(Program::Ptr p) {
    return Star::make_shared(p);
}

#include "Sequence.h"

Sequence::Sequence(Program::Ptr p1_, Program::Ptr p2_)
: p1(p1_), p2(p2_)
{
}

void Sequence::trans(State::Ptr state, ProgramStateVector &result)
{
    if(p1->isFinal(state))
    {
        ProgramStateVector result1;
        p2->trans(state, result1);
        for(ProgramState &ps : result1)
            result.push_back(ps);
    }
    else
    {
        ProgramStateVector result1;
        p1->trans(state, result1);
        for(ProgramState &ps : result1)
            result.push_back(ProgramState(Sequence::make_shared(ps.program, p2), ps.state, nullptr, ps.action));
    }
}

bool Sequence::isFinal(State::Ptr state)
{
    return p1->isFinal(state) && p2->isFinal(state);
}

string Sequence::str() const
{
    string s = "Sequence(";
    s += p1->str();
    s += ", ";
    s += p2->str();
    s += ")";
    return s;
}

Program::Ptr seq(Program::Ptr p1, Program::Ptr p2) {
    return Sequence::make_shared(p1, p2);
}

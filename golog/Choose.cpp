#include "Choose.h"

Choose::Choose(Program::Ptr p1_, Program::Ptr p2_)
: p1(p1_), p2(p2_)
{
}

void Choose::trans(State::Ptr state, ProgramStateVector &result)
{
    ProgramStateVector result1;
    p1->trans(state, result1);
    for(ProgramState &ps : result1)
        result.push_back(ps);
    
    ProgramStateVector result2;
    p2->trans(state, result2);
    for(ProgramState &ps : result2)
        result.push_back(ps);
}

bool Choose::isFinal(State::Ptr state)
{
    return p1->isFinal(state) || p2->isFinal(state);
}

string Choose::str() const
{
    string s = "Choose(";
    s += p1->str();
    s += ", ";
    s += p2->str();
    s += ")";
    return s;
}

Program::Ptr choose(Program::Ptr p1, Program::Ptr p2) {
    return Choose::make_shared(p1, p2);
}

#include "Empty.h"

Empty::Empty()
{
}

void Empty::trans(State::Ptr state, ProgramStateVector &result)
{
}

bool Empty::isFinal(State::Ptr state)
{
    return true;
}

std::string Empty::str() const
{
    return "Empty()";
}

Program::Ptr empty() {
    return Empty::make_shared();
}

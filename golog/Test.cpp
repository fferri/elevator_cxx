#include "Test.h"

Test::Test(Predicate predicate_)
: predicate(predicate_)
{
}

void Test::trans(State::Ptr state, ProgramStateVector &result)
{
    if(predicate(state))
    {
        result.push_back(ProgramState(empty(), state));
    }
}

bool Test::isFinal(State::Ptr state)
{
    return false;
}

string Test::str() const
{
    return "Test(...)";
}

Program::Ptr test(Predicate p) {
    return Test::make_shared(p);
}

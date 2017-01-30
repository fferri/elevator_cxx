#include "ProgramStateIterator.h"
#include "Program.h"

#if 0
#define TRACE_PROGRAMSTATEITERATOR_OPS
#endif

#ifdef TRACE_PROGRAMSTATEITERATOR_OPS
#include <iostream>
using std::cout;
using std::endl;
#endif

void ProgramStateIterator::expandTop()
{
    while(!s.empty())
    {
        ProgramState::Ptr ps = s.top();
        if(ps->isFinal()) break;
        s.pop();
        
#ifdef TRACE_PROGRAMSTATEITERATOR_OPS
        cout << "ProgramStateIterator:  POP " << *ps << endl;
#endif
        ProgramStateVector pss;
        ps->trans(pss);
        for(ProgramState &x : pss)
        {
            s.push(make_shared<ProgramState>(x.program, x.state, ps, x.action));
#ifdef TRACE_PROGRAMSTATEITERATOR_OPS
            cout << "ProgramStateIterator: PUSH " << *ps << endl;
#endif
        }
    }
}

ProgramStateIterator::ProgramStateIterator(ProgramState::Ptr ps0)
{
    s.push(ps0);
}

bool ProgramStateIterator::hasNext()
{
    expandTop();
    return !s.empty();
}

ProgramState::Ptr ProgramStateIterator::next()
{
    expandTop();
    ProgramState::Ptr ps = s.top(); s.pop();
    return ps;
}

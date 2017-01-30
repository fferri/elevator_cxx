#ifndef GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED
#define GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED

#include <vector>
#include <stack>

#include "ProgramState.h"

template<typename S>
class ProgramStateIterator
{
private:
    std::stack<typename ProgramState<S>::Ptr> s;
    
    void expandTop()
    {
        while(!s.empty())
        {
            typename ProgramState<S>::Ptr ps = s.top();
            if(ps->isFinal()) break;
            s.pop();
            
            ProgramStateVector<S> pss;
            ps->trans(pss);
            for(ProgramState<S> &x : pss)
            {
                s.push(make_shared<ProgramState<S>>(x.program, x.state, ps, x.action));
            }
        }
    }
    
public:
    ProgramStateIterator(typename ProgramState<S>::Ptr ps0)
    {
        s.push(ps0);
    }
    
    bool hasNext()
    {
        expandTop();
        return !s.empty();
    }
    
    typename ProgramState<S>::Ptr next()
    {
        expandTop();
        typename ProgramState<S>::Ptr ps = s.top(); s.pop();
        return ps;
    }
};

#endif // GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED


#ifndef GOLOG_STAR_H_INCLUDED
#define GOLOG_STAR_H_INCLUDED

#include "Program.h"
#include "Util.h"
#include "Empty.h"
#include "Sequence.h"

template<typename S>
class Star : public Program<S>
{
public:
    Star(typename Program<S>::Ptr p_)
    : p(p_)
    {
    }
    
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        ProgramStateVector<S> result1;
        p->trans(state, result1);
        for(ProgramState<S> &ps : result1)
            result.push_back(ProgramState<S>(std::make_shared<Sequence<S>>(ps.program, this->shared_from_this()), ps.state, nullptr, ps.action));
        result.push_back(ProgramState<S>(std::make_shared<Empty<S>>(), state));
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return false;
    }
    
    virtual string str() const
    {
        string s = "Star(";
        s += p->str();
        s += ")";
        return s;
    }
    
private:
    typename Program<S>::Ptr p;
};

#endif // GOLOG_STAR_H_INCLUDED


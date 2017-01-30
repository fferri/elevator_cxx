#ifndef GOLOG_STAR_H_INCLUDED
#define GOLOG_STAR_H_INCLUDED

#include "Program.h"
#include "Util.h"
#include "Empty.h"
#include "Sequence.h"

template<typename S>
class Star : public Program<S>, public add_make_shared<Star<S>>
{
private:
    friend class add_make_shared<Star<S>>;
    
    Star(typename Program<S>::Ptr p_)
    : p(p_)
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        ProgramStateVector<S> result1;
        p->trans(state, result1);
        for(ProgramState<S> &ps : result1)
            result.push_back(ProgramState<S>(seq<S>(ps.program, this->shared_from_this()), ps.state, nullptr, ps.action));
        result.push_back(ProgramState<S>(empty<S>(), state));
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

template<typename S>
typename Program<S>::Ptr star(typename Program<S>::Ptr p)
{
    return Star<S>::make_shared(p);
}

#endif // GOLOG_STAR_H_INCLUDED


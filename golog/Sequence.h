#ifndef GOLOG_SEQUENCE_H_INCLUDED
#define GOLOG_SEQUENCE_H_INCLUDED

#include "Program.h"
#include "Util.h"

template<typename S>
class Sequence : public Program<S>, public add_make_shared<Sequence<S>>
{
private:
    friend class add_make_shared<Sequence<S>>;
    
    Sequence(typename Program<S>::Ptr p1_, typename Program<S>::Ptr p2_)
    : p1(p1_), p2(p2_)
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        if(p1->isFinal(state))
        {
            ProgramStateVector<S> result1;
            p2->trans(state, result1);
            for(ProgramState<S> &ps : result1)
                result.push_back(ps);
        }
        else
        {
            ProgramStateVector<S> result1;
            p1->trans(state, result1);
            for(ProgramState<S> &ps : result1)
                result.push_back(ProgramState<S>(Sequence<S>::make_shared(ps.program, p2), ps.state, nullptr, ps.action));
        }
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return p1->isFinal(state) && p2->isFinal(state);
    }
    
    virtual string str() const
    {
        string s = "Sequence(";
        s += p1->str();
        s += ", ";
        s += p2->str();
        s += ")";
        return s;
    }
    
private:
    typename Program<S>::Ptr p1;
    typename Program<S>::Ptr p2;
};

template<typename S>
typename Program<S>::Ptr seq(typename Program<S>::Ptr p1, typename Program<S>::Ptr p2)
{
    return Sequence<S>::make_shared(p1, p2);
}

#endif // GOLOG_SEQUENCE_H_INCLUDED


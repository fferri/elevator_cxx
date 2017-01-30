#ifndef GOLOG_CHOOSE_H_INCLUDED
#define GOLOG_CHOOSE_H_INCLUDED

#include "Program.h"
#include "Util.h"

template<typename S>
class Choose : public Program<S>, public add_make_shared<Choose<S>>
{
private:
    friend class add_make_shared<Choose<S>>;
    
    Choose(typename Program<S>::Ptr p1_, typename Program<S>::Ptr p2_)
    : p1(p1_), p2(p2_)
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        ProgramStateVector<S> result1;
        p1->trans(state, result1);
        for(ProgramState<S> &ps : result1)
            result.push_back(ps);
        
        ProgramStateVector<S> result2;
        p2->trans(state, result2);
        for(ProgramState<S> &ps : result2)
            result.push_back(ps);
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return p1->isFinal(state) || p2->isFinal(state);
    }
    
    virtual string str() const
    {
        string s = "Choose(";
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
typename Program<S>::Ptr choose(typename Program<S>::Ptr p1, typename Program<S>::Ptr p2)
{
    return Choose<S>::make_shared(p1, p2);
}

#endif // GOLOG_CHOOSE_H_INCLUDED


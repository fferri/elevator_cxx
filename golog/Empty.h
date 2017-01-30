#ifndef GOLOG_EMPTY_H_INCLUDED
#define GOLOG_EMPTY_H_INCLUDED

#include <string>

#include "Program.h"
#include "Util.h"

template<typename S>
class Empty : public Program<S>, public add_make_shared<Empty<S>>
{
protected:
    friend class add_make_shared<Empty<S>>;
    
    Empty()
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return true;
    }
    
    virtual std::string str() const
    {
        return "Empty()";
    }
};

template<typename S>
typename Program<S>::Ptr empty()
{
    return Empty<S>::make_shared();
}

#endif // GOLOG_EMPTY_H_INCLUDED


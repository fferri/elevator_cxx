#ifndef GOLOG_EMPTY_H_INCLUDED
#define GOLOG_EMPTY_H_INCLUDED

#include <string>

#include "Program.h"
#include "Util.h"

template<typename S>
class Empty : public Program<S>
{
public:
    Empty()
    {
    }
    
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

#endif // GOLOG_EMPTY_H_INCLUDED


#ifndef GOLOG_TEST_H_INCLUDED
#define GOLOG_TEST_H_INCLUDED

#include <functional>

#include "Program.h"
#include "Util.h"
#include "Empty.h"

template<typename S>
using Predicate = std::function<bool(typename S::Ptr)>;

template<typename S>
class Test : public Program<S>
{
public:
    Test(Predicate<S> predicate_)
    : predicate(predicate_)
    {
    }
    
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        if(predicate(state))
        {
            result.push_back(ProgramState<S>(std::make_shared<Empty<S>>(), state));
        }
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return false;
    }
    
    virtual std::string str() const
    {
        return "Test(...)";
    }
    
private:
    Predicate<S> predicate;
};

#endif // GOLOG_TEST_H_INCLUDED


#ifndef GOLOG_TEST_H_INCLUDED
#define GOLOG_TEST_H_INCLUDED

#include <functional>

#include "Program.h"
#include "Util.h"
#include "Empty.h"

template<typename S>
using Predicate = std::function<bool(typename S::Ptr)>;

template<typename S>
class Test : public Program<S>, public add_make_shared<Test<S>>
{
private:
    friend class add_make_shared<Test<S>>;
    
    Test(Predicate<S> predicate_)
    : predicate(predicate_)
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        if(predicate(state))
        {
            result.push_back(ProgramState<S>(empty<S>(), state));
        }
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return false;
    }
    
    virtual string str() const
    {
        return "Test(...)";
    }
    
private:
    Predicate<S> predicate;
};

template<typename S>
typename Program<S>::Ptr test(Predicate<S> p)
{
    return Test<S>::make_shared(p);
}

#endif // GOLOG_TEST_H_INCLUDED


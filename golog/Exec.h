#ifndef GOLOG_EXEC_H_INCLUDED
#define GOLOG_EXEC_H_INCLUDED

#include <functional>

#include "PreconditionException.h"
#include "Program.h"
#include "Util.h"
#include "Empty.h"

template<typename S>
using Action = std::function<typename S::Ptr(typename S::Ptr)>;

template<typename S>
class Exec : public Program<S>, public add_make_shared<Exec<S>>
{
private:
    friend class add_make_shared<Exec<S>>;
    
    Exec(Action<S> action_)
    : action(action_)
    {
    }
    
public:
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        try
        {
            result.push_back(ProgramState<S>(empty<S>(), action(state), nullptr, this->shared_from_this()));
        }
        catch(PreconditionException &ex)
        {
        }
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return false;
    }
    
    virtual string str() const
    {
        return "Exec(...)";
    }
    
private:
    Action<S> action;
};

template<typename S>
typename Program<S>::Ptr exec(Action<S> a)
{
    return Exec<S>::make_shared(a);
}

#endif // GOLOG_EXEC_H_INCLUDED


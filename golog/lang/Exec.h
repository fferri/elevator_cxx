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
class Exec : public Program<S>
{
public:
    Exec(Action<S> action_)
    : action(action_)
    {
    }
    
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result)
    {
        try
        {
            result.push_back(ProgramState<S>(std::make_shared<Empty<S>>(), action(state), nullptr, this->shared_from_this()));
        }
        catch(PreconditionException &ex)
        {
        }
    }
    
    virtual bool isFinal(typename S::Ptr state)
    {
        return false;
    }
    
    virtual std::string str() const
    {
        return "Exec(...)";
    }
    
private:
    Action<S> action;
};

#endif // GOLOG_EXEC_H_INCLUDED


#ifndef GOLOG_EXEC_H_INCLUDED
#define GOLOG_EXEC_H_INCLUDED

#include <functional>

#include "PreconditionException.h"
#include "Program.h"
#include "Util.h"

using Action = std::function<State::Ptr(State::Ptr)>;

class Exec : public Program, public add_make_shared<Exec>
{
private:
    friend class add_make_shared<Exec>;
    
    Exec(Action action_);
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual string str() const;
    
private:
    Action action;
};

Program::Ptr exec(Action a);

#endif // GOLOG_EXEC_H_INCLUDED


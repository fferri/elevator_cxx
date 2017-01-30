#ifndef GOLOG_EMPTY_H_INCLUDED
#define GOLOG_EMPTY_H_INCLUDED

#include <string>

#include "Program.h"
#include "Util.h"

class Empty : public Program, public add_make_shared<Empty>
{
protected:
    friend class add_make_shared<Empty>;
    
    Empty();
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual std::string str() const;
};

Program::Ptr empty();

#endif // GOLOG_EMPTY_H_INCLUDED


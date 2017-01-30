#ifndef GOLOG_PROGRAM_H_INCLUDED
#define GOLOG_PROGRAM_H_INCLUDED

#include <memory>
#include <string>

#include "ProgramState.h"

class Program
{
public:
    using Ptr = std::shared_ptr<Program>;
    
    virtual void trans(State::Ptr state, ProgramStateVector &result) = 0;
    
    virtual bool isFinal(State::Ptr state) = 0;
    
    virtual std::string str() const = 0;
};

ostream& operator<<(ostream &os, const Program &p);

#endif // GOLOG_PROGRAM_H_INCLUDED


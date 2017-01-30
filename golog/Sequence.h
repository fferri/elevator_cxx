#ifndef GOLOG_SEQUENCE_H_INCLUDED
#define GOLOG_SEQUENCE_H_INCLUDED

#include "Program.h"
#include "Util.h"

class Sequence : public Program, public add_make_shared<Sequence>
{
private:
    friend class add_make_shared<Sequence>;
    
    Sequence(Program::Ptr p1_, Program::Ptr p2_);
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual string str() const;
    
private:
    Program::Ptr p1;
    Program::Ptr p2;
};

Program::Ptr seq(Program::Ptr p1, Program::Ptr p2);

#endif // GOLOG_SEQUENCE_H_INCLUDED


#ifndef GOLOG_CHOOSE_H_INCLUDED
#define GOLOG_CHOOSE_H_INCLUDED

#include "Program.h"
#include "Util.h"

class Choose : public Program, public add_make_shared<Choose>
{
private:
    friend class add_make_shared<Choose>;
    
    Choose(Program::Ptr p1_, Program::Ptr p2_);
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual string str() const;
    
private:
    Program::Ptr p1;
    Program::Ptr p2;
};

Program::Ptr choose(Program::Ptr p1, Program::Ptr p2);

#endif // GOLOG_CHOOSE_H_INCLUDED


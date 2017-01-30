#ifndef GOLOG_STAR_H_INCLUDED
#define GOLOG_STAR_H_INCLUDED

#include "Program.h"
#include "Util.h"

class Star : public Program, public add_make_shared<Star>
{
private:
    friend class add_make_shared<Star>;
    
    Star(Program::Ptr p_);
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual string str() const;
    
private:
    Program::Ptr p;
};

Program::Ptr star(Program::Ptr p);

#endif // GOLOG_STAR_H_INCLUDED


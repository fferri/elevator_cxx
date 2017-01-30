#ifndef GOLOG_TEST_H_INCLUDED
#define GOLOG_TEST_H_INCLUDED

#include <functional>

#include "Program.h"
#include "Util.h"
#include "Empty.h"

using Predicate = std::function<bool(State::Ptr)>;

class Test : public Program, public add_make_shared<Test>
{
private:
    friend class add_make_shared<Test>;
    
    Test(Predicate predicate_);
    
public:
    virtual void trans(State::Ptr state, ProgramStateVector &result);
    
    virtual bool isFinal(State::Ptr state);
    
    virtual string str() const;
    
private:
    Predicate predicate;
};

Program::Ptr test(Predicate p);

#endif // GOLOG_TEST_H_INCLUDED


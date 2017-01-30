#ifndef GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED
#define GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED

#include <vector>
#include <stack>

#include "ProgramState.h"

class ProgramStateIterator
{
private:
    std::stack<ProgramState::Ptr> s;
    
    void expandTop();
    
public:
    ProgramStateIterator(ProgramState::Ptr ps0);
    
    bool hasNext();
    
    ProgramState::Ptr next();
};

#endif // GOLOG_PROGRAMSTATEITERATOR_H_INCLUDED


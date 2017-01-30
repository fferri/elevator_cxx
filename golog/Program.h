#ifndef GOLOG_PROGRAM_H_INCLUDED
#define GOLOG_PROGRAM_H_INCLUDED

#include <memory>
#include <string>

#include "ProgramState.h"

template<typename S>
class Program : public std::enable_shared_from_this<Program<S>>
{
public:
    using Ptr = std::shared_ptr<Program>;
    
    virtual void trans(typename S::Ptr state, ProgramStateVector<S> &result) = 0;
    
    virtual bool isFinal(typename S::Ptr state) = 0;
    
    virtual std::string str() const = 0;
};

template<typename S>
std::ostream& operator<<(std::ostream &os, const Program<S> &p)
{
    os << p.str();
    return os;
}

#endif // GOLOG_PROGRAM_H_INCLUDED


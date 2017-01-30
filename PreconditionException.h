#ifndef PRECONDITIONEXCEPTION_H_INCLUDED
#define PRECONDITIONEXCEPTION_H_INCLUDED

#include <string>
#include <exception>

class PreconditionException : public std::runtime_error
{
    std::string msg;
    
public:
    PreconditionException(const std::string &msg_);
    virtual const char* what() const throw();
};

#endif // PRECONDITIONEXCEPTION_H_INCLUDED


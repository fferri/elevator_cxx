#ifndef PRECONDITIONEXCEPTION_H_INCLUDED
#define PRECONDITIONEXCEPTION_H_INCLUDED

#include <string>
#include <exception>

using std::string;
using std::runtime_error;

class PreconditionException : public runtime_error
{
    string msg;
public:
    PreconditionException(const string &msg_);
    virtual const char* what() const throw();
};

#endif // PRECONDITIONEXCEPTION_H_INCLUDED


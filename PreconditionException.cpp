#include "PreconditionException.h"

#include <sstream>

using std::stringstream;

PreconditionException::PreconditionException(const string &msg_)
    : msg(msg_), runtime_error(what())
{
}

const char * PreconditionException::what() const throw()
{
    stringstream ss;
    ss << "PreconditionException";
    if(msg.size() > 0)
    {
        ss << ": " << msg;
    }
    return ss.str().c_str();
}


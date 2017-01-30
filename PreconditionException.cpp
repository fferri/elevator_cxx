#include "PreconditionException.h"

#include <sstream>

PreconditionException::PreconditionException(const std::string &msg_)
: msg(msg_), std::runtime_error(what())
{
}

const char * PreconditionException::what() const throw()
{
    std::stringstream ss;
    ss << "PreconditionException";
    if(msg.size() > 0)
    {
        ss << ": " << msg;
    }
    return ss.str().c_str();
}


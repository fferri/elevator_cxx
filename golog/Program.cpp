#include "Program.h"

ostream& operator<<(ostream &os, const Program &p)
{
    os << p.str();
    return os;
}

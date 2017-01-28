#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <memory>

template<typename T>
class add_make_shared : public std::enable_shared_from_this<T>
{
public:
    typedef std::shared_ptr<T> Ptr;
    
    template<typename... Args>
    static Ptr make_shared(Args&&... args)
    {
        return Ptr(new T(std::forward<Args>(args)...));
    }
};

#endif // UTIL_H_INCLUDED


#ifndef HANDLE_PTR_HPP
#define HANDLE_PTR_HPP

#include <memory>
#include <type_traits>

void DeleteGdiObj(void* obj);
void DeleteWinapiObj(void* obj);

template<typename T>
class gdi_handle_ptr :
        public std::shared_ptr<typename std::remove_pointer<T>::type>
{
    typedef std::shared_ptr<typename std::remove_pointer<T>::type> shared_ptr_t;
public:
    gdi_handle_ptr(T ptr = nullptr):
        shared_ptr_t(ptr, &DeleteGdiObj) {}
};

template<typename T>
class handle_ptr :
        public std::shared_ptr<typename std::remove_pointer<T>::type>
{
    typedef std::shared_ptr<typename std::remove_pointer<T>::type> shared_ptr_t;
public:
    handle_ptr(T ptr = nullptr):
        shared_ptr_t(ptr, &DeleteWinapiObj) {}
};

#endif // HANDLE_PTR_HPP

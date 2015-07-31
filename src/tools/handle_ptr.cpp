#include "handle_ptr.hpp"

#include <windows.h>

void DeleteGdiObj(void* obj)
{
    if(nullptr != obj)
    {
        ::DeleteObject(obj);
    }
}

void DeleteWinapiObj(void* obj)
{
    if(nullptr != obj)
    {
        ::CloseHandle(obj);
    }
}

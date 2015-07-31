#ifndef HELP_FUNCS_HPP
#define HELP_FUNCS_HPP

#include <algorithm>

namespace help_funcs
{

template<typename T>
void fill_rect(void* data,
               int x, int y,
               int width, int height,
               size_t pitch,
               const T& value)
{
    unsigned char* p = reinterpret_cast<unsigned char*>(data);
    unsigned char* start = p + y * pitch + x * sizeof(T);
    for(int i = 0; i < height; ++i)
    {
        std::fill_n(reinterpret_cast<T*>(start), width, value);
        start += pitch;
    }
}

template<typename DstT, typename SrcT>
DstT union_cast(const SrcT& val)
{
    union
    {
        SrcT s;
        DstT d;
    } u;
    u.s = val;
    return u.d;
}

}

#endif // HELP_FUNCS_HPP

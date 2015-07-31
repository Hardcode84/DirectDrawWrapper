#ifndef COMMON_HPP
#define COMMON_HPP

#include "ddraw.h"
#include <atomic>
#include <algorithm>

#include "simple_logger/logger.hpp"
#include "tools/com_ptr.hpp"

#define DD_API __stdcall

struct winerr_wrapper
{
    DWORD err;
    winerr_wrapper(DWORD e):err(e) {}
};

winerr_wrapper getWinError();
winerr_wrapper getWinErrorFromHr(HRESULT hr);

template<typename STREAM>
STREAM& operator<<(STREAM& s, const GUID& g)
{
    s << (void*)g.Data1 << '-' << (void*)g.Data2 << '-' << (void*)g.Data3;
    for(const auto c: g.Data4)
    {
        s << '-';
        s << (void*)c;
    }
    return s;
}

template<typename STREAM>
STREAM& operator<<(STREAM& s, const DDCOLORKEY& ck)
{
    s << "Color key: " << ck.dwColorSpaceLowValue << "-" << ck.dwColorSpaceHighValue;
    return s;
}

template<typename STREAM>
STREAM& operator<<(STREAM& s, const RECT& rc)
{
    s << "RECT: left: " << rc.left << " right: " << rc.right << " top: " << rc.top << " bottom: " << rc.bottom;
    return s;
}

template<typename STREAM>
STREAM& operator<<(STREAM& s, const winerr_wrapper& e)
{
    LPTSTR lpMsgBuf = nullptr;
    const DWORD size = ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            e.err,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, nullptr);
    for(DWORD i = 0; i < size; ++i)
    {
        if(('\r' == lpMsgBuf[i]) || ('\n' == lpMsgBuf[i]))
        {
            lpMsgBuf[i] = ' ';
        }
    }
    s << '\"' << lpMsgBuf << "\", code " << e.err;
    LocalFree(lpMsgBuf);
    return s;
}

#define LOG_ERROR()   WRITE_LOG(*mLog,logger::logERROR)
#define LOG_WARNING() WRITE_LOG(*mLog,logger::logWARNING)
#define LOG_INFO()    WRITE_LOG(*mLog,logger::logINFO)
#define LOG_DEBUG()   WRITE_LOG(*mLog,logger::logDEBUG)

#ifdef __GNUC__
#define FUNC_NAME __PRETTY_FUNCTION__
#else
#define FUNC_NAME __func__
#endif



#define WRITE_VAR(var)       LOG_INFO()  << #var << "=\"" << var << '\"'
#define WRITE_VAR_DEBUG(var) LOG_DEBUG() << #var << "=\"" << var << '\"'
#define WRITE_FLAG(var,flag) if((var) & (flag)) do{ LOG_INFO() << #var ": \"" #flag << "\" set"; }while(false)
#define WRITE_THIS() WRITE_VAR_DEBUG(this)

#define LOG_STATIC_FUNCTION() LOG_SCOPE((FUNC_NAME),*mLog,logger::logDEBUG);

#define LOG_FUNCTION() LOG_SCOPE((FUNC_NAME),*mLog,logger::logDEBUG); \
    WRITE_THIS()
#define LOG_FUNCTION_E() LOG_SCOPE((FUNC_NAME),*mLog,logger::logINFO); \
    WRITE_THIS(); \
    LOG_ERROR() << (FUNC_NAME) << " not implemented;"; return E_NOTIMPL;

#define CHECK_PARAM(param) do{ if(!(param)) { LOG_ERROR() << "Param \"" #param "\" failed"; return DDERR_INVALIDPARAMS; } }while(false)

template<typename BaseT, typename SrcT>
BaseT* adjust_pointer(SrcT* src)
{
    if(nullptr == src) return nullptr;
    const BaseT* dummy1 = nullptr;
    const SrcT*  dummy2 = static_cast<const SrcT*>(dummy1);
    const auto offset = reinterpret_cast<const char*>(dummy2) - reinterpret_cast<const char*>(dummy1);
    char* p = reinterpret_cast<char*>(src);
    p -= offset;
    return reinterpret_cast<BaseT*>(p);
}

#endif // COMMON_HPP

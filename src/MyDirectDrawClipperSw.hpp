#ifndef MYDIRECTDRAWCLIPPERSW_HPP
#define MYDIRECTDRAWCLIPPERSW_HPP

#include "common.hpp"

class MyDirectDrawClipperSw :
        public IDirectDrawClipper
{
    std::atomic<ULONG> mRefCount;
    logger* mLog = nullptr;
    HWND mHWnd = nullptr;
public:
    MyDirectDrawClipperSw(logger* log);
    virtual ~MyDirectDrawClipperSw();

    HRESULT DD_API QueryInterface(const IID &riid, LPVOID *ppvObj);
    ULONG   DD_API AddRef();
    ULONG   DD_API Release();

    HRESULT DD_API GetClipList(LPRECT, LPRGNDATA, LPDWORD);
    HRESULT DD_API GetHWnd(HWND *);
    HRESULT DD_API Initialize(LPDIRECTDRAW, DWORD);
    HRESULT DD_API IsClipListChanged(WINBOOL *);
    HRESULT DD_API SetClipList(LPRGNDATA, DWORD);
    HRESULT DD_API SetHWnd(DWORD, HWND);
};

#endif // MYDIRECTDRAWCLIPPERSW_HPP

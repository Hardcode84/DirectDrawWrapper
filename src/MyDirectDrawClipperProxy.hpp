#ifndef MYDIRECTDRAWCLIPPERPROXY_HPP
#define MYDIRECTDRAWCLIPPERPROXY_HPP

#include "common.hpp"

class MyDirectDrawClipperProxy :
        public IDirectDrawClipper
{
    std::atomic<ULONG> mRefCount;
    logger* mLog = nullptr;
    com_ptr<IDirectDrawClipper> mClipper;
public:
    MyDirectDrawClipperProxy(logger* log, LPDIRECTDRAWCLIPPER c);
    virtual ~MyDirectDrawClipperProxy();

    LPDIRECTDRAWCLIPPER getSrcClipper() const { return mClipper.get(); }

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

#endif // MYDIRECTDRAWCLIPPERPROXY_HPP

#ifndef MYDIRECTDRAWPALETTEPROXY_HPP
#define MYDIRECTDRAWPALETTEPROXY_HPP

#include "common.hpp"

class MyDirectDrawPaletteProxy :
        public IDirectDrawPalette
{
    std::atomic<ULONG> mRefCount;
    logger* mLog = nullptr;
    com_ptr<IDirectDrawPalette> mPalette;
public:
    MyDirectDrawPaletteProxy(logger* log, LPDIRECTDRAWPALETTE p);
    virtual ~MyDirectDrawPaletteProxy();

    LPDIRECTDRAWPALETTE GetSrcPalette() const { return mPalette.get(); }

    HRESULT DD_API QueryInterface(const IID &riid, LPVOID *ppvObj);
    ULONG   DD_API AddRef();
    ULONG   DD_API Release();

    HRESULT DD_API GetCaps(LPDWORD);
    HRESULT DD_API GetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY);
    HRESULT DD_API Initialize(LPDIRECTDRAW, DWORD, LPPALETTEENTRY);
    HRESULT DD_API SetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY);
};

#endif // MYDIRECTDRAWPALETTEPROXY_HPP

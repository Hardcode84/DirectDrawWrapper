#include "MyDirectDrawPaletteProxy.hpp"

MyDirectDrawPaletteProxy::MyDirectDrawPaletteProxy(logger *log, LPDIRECTDRAWPALETTE p):
    mRefCount(1),
    mLog(log),
    mPalette(p)
{
    LOG_FUNCTION();
}

MyDirectDrawPaletteProxy::~MyDirectDrawPaletteProxy()
{
    LOG_FUNCTION();
}

HRESULT DD_API MyDirectDrawPaletteProxy::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();
    return mPalette->QueryInterface(riid,ppvObj);
}

ULONG   DD_API MyDirectDrawPaletteProxy::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawPaletteProxy::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

HRESULT DD_API MyDirectDrawPaletteProxy::GetCaps(LPDWORD lpdwCaps)
{
    LOG_FUNCTION();
    return mPalette->GetCaps(lpdwCaps);
}

HRESULT DD_API MyDirectDrawPaletteProxy::GetEntries(DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
    LOG_FUNCTION();
    return mPalette->GetEntries(dwFlags, dwBase, dwNumEntries, lpEntries);
}

HRESULT DD_API MyDirectDrawPaletteProxy::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable)
{
    LOG_FUNCTION();
    return mPalette->Initialize(lpDD, dwFlags, lpDDColorTable);
}

HRESULT DD_API MyDirectDrawPaletteProxy::SetEntries(DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
    LOG_FUNCTION();
    return mPalette->SetEntries(dwFlags, dwStartingEntry, dwCount, lpEntries);
}

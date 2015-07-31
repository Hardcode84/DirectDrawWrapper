#include "MyDirectDrawClipperProxy.hpp"

MyDirectDrawClipperProxy::MyDirectDrawClipperProxy(logger *log, LPDIRECTDRAWCLIPPER c):
    mRefCount(1),
    mLog(log),
    mClipper(c)
{
    LOG_FUNCTION();
}

MyDirectDrawClipperProxy::~MyDirectDrawClipperProxy()
{
    LOG_FUNCTION();
}

HRESULT DD_API MyDirectDrawClipperProxy::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();
    return mClipper->QueryInterface(riid,ppvObj);
}

ULONG   DD_API MyDirectDrawClipperProxy::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawClipperProxy::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

HRESULT DD_API MyDirectDrawClipperProxy::GetClipList(LPRECT lpRect, LPRGNDATA lpClipList, LPDWORD lpdwSize)
{
    LOG_FUNCTION();
    return mClipper->GetClipList(lpRect, lpClipList, lpdwSize);
}
HRESULT DD_API MyDirectDrawClipperProxy::GetHWnd(HWND *lphWnd)
{
    LOG_FUNCTION();
    return mClipper->GetHWnd(lphWnd);
}
HRESULT DD_API MyDirectDrawClipperProxy::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mClipper->Initialize(lpDD, dwFlags);
}
HRESULT DD_API MyDirectDrawClipperProxy::IsClipListChanged(WINBOOL *lpbChanged)
{
    LOG_FUNCTION();
    return mClipper->IsClipListChanged(lpbChanged);
}
HRESULT DD_API MyDirectDrawClipperProxy::SetClipList(LPRGNDATA lpClipList, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mClipper->SetClipList(lpClipList, dwFlags);
}
HRESULT DD_API MyDirectDrawClipperProxy::SetHWnd(DWORD dwFlags, HWND hWnd)
{
    LOG_FUNCTION();
    WRITE_VAR(hWnd);
    return mClipper->SetHWnd(dwFlags, hWnd);
}

#include "MyDirectDrawClipperSw.hpp"

MyDirectDrawClipperSw::MyDirectDrawClipperSw(logger *log):
    mRefCount(1),
    mLog(log)
{
    LOG_FUNCTION();
}

MyDirectDrawClipperSw::~MyDirectDrawClipperSw()
{
    LOG_FUNCTION();
}

HRESULT DD_API MyDirectDrawClipperSw::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();
    *ppvObj = nullptr;
    return E_NOINTERFACE;
}

ULONG   DD_API MyDirectDrawClipperSw::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawClipperSw::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

HRESULT DD_API MyDirectDrawClipperSw::GetClipList(LPRECT lpRect, LPRGNDATA lpClipList, LPDWORD lpdwSize)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawClipperSw::GetHWnd(HWND *lphWnd)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lphWnd);
    *lphWnd = mHWnd;
    return S_OK;
}
HRESULT DD_API MyDirectDrawClipperSw::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags)
{
    LOG_FUNCTION();
    (void)lpDD;
    CHECK_PARAM(0 == dwFlags);
    return DDERR_ALREADYINITIALIZED;
}
HRESULT DD_API MyDirectDrawClipperSw::IsClipListChanged(WINBOOL *lpbChanged)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawClipperSw::SetClipList(LPRGNDATA lpClipList, DWORD dwFlags)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawClipperSw::SetHWnd(DWORD dwFlags, HWND hWnd)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    mHWnd = hWnd;
    return S_OK;
}

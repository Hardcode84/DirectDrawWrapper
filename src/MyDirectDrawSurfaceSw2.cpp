#include "MyDirectDrawSurfaceSw.hpp"

#include "MyDirectDrawSw.hpp"

//IDirectDrawSurface2
HRESULT DD_API MyDirectDrawSurfaceSw::AddAttachedSurface(LPDIRECTDRAWSURFACE2 lpDDSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    return BltInternal(lpDestRect, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceSw::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    return BltFastInternal(dwX, dwY, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Flip(LPDIRECTDRAWSURFACE2 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return FlipInternal(adjPtr(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE2 *lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDSCaps);
    CHECK_PARAM(nullptr != lplpDDAttachedSurface);
    MyDirectDrawSurfaceSw* ret = GetAttachedSurfaceInternal(lpDDSCaps);
    if(nullptr != ret)
    {
        *lplpDDAttachedSurface = ret;
        return S_OK;
    }
    return DDERR_NOTFOUND;
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE2 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSReference)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetDDInterface(LPVOID * lplpDD)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lplpDD);
    *lplpDD = static_cast<LPDIRECTDRAW>(mParent);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::PageLock(DWORD dwFlags)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    if(0 == mPageLockCount++)
    {
        ::VirtualLock(GetBufferPtr(), bufferSize());
    }
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::PageUnlock(DWORD dwFlags)
{
    LOG_FUNCTION();
    CHECK_PARAM(isPageLocked());
    CHECK_PARAM(0 == dwFlags);
    if(0 == --mPageLockCount)
    {
        ::VirtualUnlock(GetBufferPtr(), bufferSize());
        OnChangedInternal();
    }
    return S_OK;
}

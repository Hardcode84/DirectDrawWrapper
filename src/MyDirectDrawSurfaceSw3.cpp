#include "MyDirectDrawSurfaceSw.hpp"

//IDirectDrawSurface3
HRESULT DD_API MyDirectDrawSurfaceSw::AddAttachedSurface(LPDIRECTDRAWSURFACE3 lpDDSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE3 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    return BltInternal(lpDestRect, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceSw::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE3 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    return BltFastInternal(dwX, dwY, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE3 lpDDSAttachedSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Flip(LPDIRECTDRAWSURFACE3 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return FlipInternal(adjPtr(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE3 * lplpDDAttachedSurface)
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
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE3 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE3 lpDDSReference)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::SetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags)
{
    LOG_FUNCTION_E();
}

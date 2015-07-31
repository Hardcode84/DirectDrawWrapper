#include "MyDirectDrawSurfaceSw.hpp"

//IDirectDrawSurface
HRESULT DD_API MyDirectDrawSurfaceSw::AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::AddOverlayDirtyRect(LPRECT lpRect)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    return BltInternal(lpDestRect, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceSw::BltBatch(LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    return BltFastInternal(dwX, dwY, adjPtr(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpfnCallback)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::Flip(LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return FlipInternal(adjPtr(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE * lplpDDAttachedSurface)
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
HRESULT DD_API MyDirectDrawSurfaceSw::GetBltStatus(DWORD dwFlags)
{
    LOG_FUNCTION();
    (void)dwFlags;
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetCaps(LPDDSCAPS lpDDSCaps)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDSCaps);
    GetCapsInternal(lpDDSCaps);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetClipper(LPDIRECTDRAWCLIPPER *lplpDDClipper)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lplpDDClipper);
    if(mClipper)
    {
        mClipper->AddRef();
    }
    *lplpDDClipper = mClipper.get();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDColorKey);
    CHECK_PARAM(DDCKEY_DESTOVERLAY ^ dwFlags);
    CHECK_PARAM(DDCKEY_SRCOVERLAY ^ dwFlags);
    if(DDCKEY_DESTBLT & dwFlags)
    {
        *lpDDColorKey = mDstColorKey;
    }
    else if(DDCKEY_SRCBLT & dwFlags)
    {
        *lpDDColorKey = mSrcColorKey;
    }
    else
    {
        LOG_ERROR() << "You must specify DDCKEY_DESTBLT or DDCKEY_SRCBLT";
        return DDERR_INVALIDPARAMS;
    }
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetDC(HDC *lphDC)
{
    LOG_FUNCTION();
    return GetDCInternal(lphDC);
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetFlipStatus(DWORD dwFlags)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetOverlayPosition(LPLONG lplX, LPLONG lplY)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetPalette(LPDIRECTDRAWPALETTE * lplpDDPalette)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lplpDDPalette);
    if(mPalette)
    {
        mPalette->AddRef();
    }
    *lplpDDPalette = mPalette.get();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDPixelFormat);
    GetPixelFormatInternal(lpDDPixelFormat);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDSurfaceDesc);
    GetDescInternal(lpDDSurfaceDesc);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::Initialize(LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return DDERR_ALREADYINITIALIZED;
}
HRESULT DD_API MyDirectDrawSurfaceSw::IsLost()
{
    LOG_FUNCTION();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr == hEvent);
    return LockInternal(lpDestRect, lpDDSurfaceDesc, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceSw::ReleaseDC(HDC hDC)
{
    LOG_FUNCTION();
    return ReleaseDCInternal(hDC);
}
HRESULT DD_API MyDirectDrawSurfaceSw::Restore()
{
    LOG_FUNCTION();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
{
    LOG_FUNCTION();
    mClipper.reset(adjust_pointer<MyDirectDrawClipperSw>(lpDDClipper),true);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::SetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpDDColorKey);
    CHECK_PARAM(DDCKEY_DESTOVERLAY ^ dwFlags);
    CHECK_PARAM(DDCKEY_SRCOVERLAY ^ dwFlags);
    if(DDCKEY_DESTBLT & dwFlags)
    {
        mDstColorKey = *lpDDColorKey;
    }
    else if(DDCKEY_SRCBLT & dwFlags)
    {
        mSrcColorKey = *lpDDColorKey;
    }
    else
    {
        LOG_ERROR() << "You must specify DDCKEY_DESTBLT or DDCKEY_SRCBLT";
        return DDERR_INVALIDPARAMS;
    }
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::SetOverlayPosition(LONG lX, LONG lY)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
    LOG_FUNCTION();
    CHECK_PARAM(!isDCOut());
    mPalette.reset(adjust_pointer<MyDirectDrawPaletteSw>(lpDDPalette), true);
    mHBitmap.reset();
    mPaletteStamp = 0;
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceSw::Unlock(LPVOID lpData)
{
    LOG_FUNCTION();
    (void)lpData;
    return UnlockInternal();
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlayDisplay(DWORD dwFlags)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSurfaceSw::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE lpDDSReference)
{
    LOG_FUNCTION_E();
}

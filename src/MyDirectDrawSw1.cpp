#include "MyDirectDrawSw.hpp"

#include "MyDirectDrawPaletteSw.hpp"
#include "MyDirectDrawClipperSw.hpp"
#include "MyDirectDrawSurfaceSw.hpp"

//IDirectDraw
HRESULT DD_API MyDirectDrawSw::Compact()
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER * lplpDDClipper, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    CHECK_PARAM(nullptr != lplpDDClipper);
    CHECK_PARAM(nullptr == pUnkOuter);
    *lplpDDClipper = new MyDirectDrawClipperSw(mLog);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, LPDIRECTDRAWPALETTE * lplpDDPalette, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr == pUnkOuter);
    CHECK_PARAM(nullptr != lpDDColorArray);
    CHECK_PARAM(nullptr != lplpDDPalette);
    *lplpDDPalette = new MyDirectDrawPaletteSw(mLog, dwFlags, lpDDColorArray, this);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE * lplpDDSurface, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr == pUnkOuter);
    CHECK_PARAM(nullptr != lpDDSurfaceDesc);
    CHECK_PARAM(nullptr != lplpDDSurface);
    const auto ret = createSurfaceInternal(lpDDSurfaceDesc);
    *lplpDDSurface = ret;
    return (nullptr != ret ? S_OK : E_FAIL);
}
HRESULT DD_API MyDirectDrawSw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE * lplpDupDDSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    CHECK_PARAM(nullptr == lpDDSurfaceDesc);
    enumDispModesInternal(lpContext, lpEnumModesCallback);
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::FlipToGDISurface()
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDEmulCaps)
{
    LOG_FUNCTION();
    if(nullptr != lpDDDriverCaps)
    {
        getDescInternal(lpDDDriverCaps);
    }
    if(nullptr != lpDDEmulCaps)
    {
        getDescInternal(lpDDEmulCaps);
    }
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetGDISurface(LPDIRECTDRAWSURFACE *lplpGDIDDSSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetMonitorFrequency(LPDWORD lpdwFrequency)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetScanLine(LPDWORD lpdwScanLine)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetVerticalBlankStatus(LPBOOL lpbIsInVB)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpbIsInVB);
    *lpbIsInVB = TRUE;
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::Initialize(GUID* g)
{
    LOG_FUNCTION();
    (void)g;
    return DDERR_ALREADYINITIALIZED;
}
HRESULT DD_API MyDirectDrawSw::RestoreDisplayMode()
{
    LOG_FUNCTION();
    restoreDisplayModeInternal();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
    LOG_FUNCTION();
    const bool result = setWindow(hWnd, dwFlags & DDSCL_FULLSCREEN);
    return (result ? S_OK : E_FAIL);
}
HRESULT DD_API MyDirectDrawSw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
    LOG_FUNCTION();
    const bool result = setDisplayModeInternal(DisplayMode(dwWidth, dwHeight, dwBPP));
    return (result ? S_OK : E_FAIL);
}
HRESULT DD_API MyDirectDrawSw::WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr == hEvent);
    (void)dwFlags;
    return S_OK;
}

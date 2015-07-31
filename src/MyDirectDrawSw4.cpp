#include "MyDirectDrawSw.hpp"

//IDirectDraw4
HRESULT DD_API MyDirectDrawSw::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPDIRECTDRAWSURFACE4 *lplpDDSurface, IUnknown *pUnkOuter)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::DuplicateSurface(LPDIRECTDRAWSURFACE4 lpDDSurface, LPDIRECTDRAWSURFACE4 *lplpDupDDSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetGDISurface(LPDIRECTDRAWSURFACE4 *lplpGDIDDSSurface)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetAvailableVidMem(LPDDSCAPS2 lpDDSCaps, LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetSurfaceFromDC(HDC hdc, LPDIRECTDRAWSURFACE4 *lpDDS)
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::RestoreAllSurfaces()
{
    LOG_FUNCTION();
    return S_OK;
}
HRESULT DD_API MyDirectDrawSw::TestCooperativeLevel()
{
    LOG_FUNCTION_E();
}
HRESULT DD_API MyDirectDrawSw::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER lpDDDeviceIdentifier, DWORD dwFlags)
{
    LOG_FUNCTION_E();
}

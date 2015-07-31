#include "MyDirectDrawSw.hpp"

//IDirectDraw2
HRESULT DD_API MyDirectDrawSw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    const bool result = setDisplayModeInternal(DisplayMode(dwWidth, dwHeight, dwBPP, dwRefreshRate));
    return (result ? S_OK : E_FAIL);
}
HRESULT DD_API MyDirectDrawSw::GetAvailableVidMem(LPDDSCAPS lpDDSCaps, LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
    LOG_FUNCTION_E();
}

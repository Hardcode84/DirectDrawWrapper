#ifndef LIBMAIN_HPP
#define LIBMAIN_HPP

#include <windows.h>
#include "ddraw.h"
#include "ddraw_export.h"

// Exported function

extern "C"
{
DDRAW_EXPORT HRESULT WINAPI DirectDrawCreate(GUID *lpGUID, LPDIRECTDRAW *lplpDD, IUnknown *pUnkOuter);
DDRAW_EXPORT HRESULT WINAPI DirectDrawCreateEx(GUID *lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown *pUnkOuter);

//HRESULT WINAPI DirectDrawCreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER FAR *lplpDDClipper, IUnknown *pUnkOuter);

DDRAW_EXPORT HRESULT WINAPI DirectDrawEnumerateW(LPDDENUMCALLBACKW lpCallback, LPVOID lpContext);
DDRAW_EXPORT HRESULT WINAPI DirectDrawEnumerateA(LPDDENUMCALLBACKA lpCallback, LPVOID lpContext);
DDRAW_EXPORT HRESULT WINAPI DirectDrawEnumerateExW(LPDDENUMCALLBACKEXW lpCallback, LPVOID lpContext, DWORD dwFlags);
DDRAW_EXPORT HRESULT WINAPI DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags);

//DDRAW_EXPORT VOID WINAPI AcquireDDThreadLock(void);
//DDRAW_EXPORT VOID WINAPI ReleaseDDThreadLock(void);
//DDRAW_EXPORT DWORD WINAPI D3DParseUnknownCommand(LPVOID lpCmd, LPVOID *lpRetCmd);
//DDRAW_EXPORT HRESULT WINAPI DllCanUnloadNow(void);
//DDRAW_EXPORT HRESULT WINAPI DllGetClassObject (const CLSID &rclsid, const IID &riid, void **ppv);
/*
// Exported functions still missing

GetSurfaceFromDC
CheckFullscreen
CompleteCreateSysmemSurface
DDGetAttachedSurfaceLcl
DDInternalLock
DDInternalUnlock
DSoundHelp
GetDDSurfaceLocal
GetOLEThunkData
RegisterSpecialCase
*/
}
// regular functions
bool InitInstance(HANDLE hModule);
void ExitInstance(void);
bool LoadOriginalDll(void);


#endif // LIBMAIN_HPP

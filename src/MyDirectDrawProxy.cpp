#include "MyDirectDrawProxy.hpp"

#include "serialize_helpers.hpp"

#include "MyDirectDrawSurfaceProxy.hpp"
#include "MyDirectDrawPaletteProxy.hpp"
#include "MyDirectDrawClipperProxy.hpp"

MyDirectDrawProxy::MyDirectDrawProxy(logger *log, LPDIRECTDRAW dd):
    mRefCount(1),
    mLog(log),
    mDD(dd)
{
    LOG_FUNCTION();
}

MyDirectDrawProxy::~MyDirectDrawProxy()
{
    LOG_FUNCTION();
}

HRESULT WINAPI MyDirectDrawProxy::EnumModesCallback(LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext)
{
    const DmEnumContext* cx = reinterpret_cast<const DmEnumContext*>(lpContext);
    write_log(lpDDSurfaceDesc, cx->obj->mLog);
    return cx->callback(lpDDSurfaceDesc, cx->origContext);
}

DEFINE_GUID( IID_IDirect3DNullDevice, 0x8767df22, 0xbacc, 0x11d1, 0x89, 0x69, 0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);
DEFINE_GUID( IID_IDirect3D7,          0xf5049e77, 0x4861, 0x11d2, 0xa4, 0x7,  0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);

HRESULT DD_API MyDirectDrawProxy::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();

    const std::pair<int,IID> versions[] = {{1,IID_IDirectDraw},
                                           {2,IID_IDirectDraw2},
                                           {4,IID_IDirectDraw4},
                                           {7,IID_IDirectDraw7},
                                           {7,IID_IDirect3D7}};

    const auto ver = std::find_if(std::begin(versions),
                                  std::end(versions),
                                  [&](const std::pair<int,GUID>& val)
    {
        return riid == val.second;
    });

    if(std::end(versions) == ver)
    {
        mVersion = 1;
        LOG_ERROR() << "Unknown DD version. IID: " << riid;
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }
    else
    {
        mVersion = (*ver).first;
        LOG_INFO() << "DD version: " << mVersion;
    }

    AddRef();
    if(mVersion >= 1)
    {
        *ppvObj = static_cast<LPDIRECTDRAW>(this);
    }
    if(mVersion >= 2)
    {
        if(!mDD2)
        {
            const HRESULT hr = mDD->QueryInterface(IID_IDirectDraw2,(void**)&mDD2);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAW2>(this);
    }
    if(mVersion >= 4)
    {
        if(!mDD4)
        {
            const HRESULT hr = mDD->QueryInterface(IID_IDirectDraw4,(void**)&mDD4);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAW4>(this);
    }
    if(mVersion >= 7)
    {
        if(!mDD7)
        {
            const HRESULT hr = mDD->QueryInterface(IID_IDirectDraw7,(void**)&mDD7);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAW7>(this);
    }
    return S_OK;
}

ULONG   DD_API MyDirectDrawProxy::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawProxy::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}


//IDirectDraw
HRESULT DD_API MyDirectDrawProxy::Compact()
{
    LOG_FUNCTION();
    return mDD->Compact();
}
HRESULT DD_API MyDirectDrawProxy::CreateClipper(DWORD dwFlags, LPDIRECTDRAWCLIPPER * lplpDDClipper, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    LPDIRECTDRAWCLIPPER temp;
    const HRESULT hr =  mDD->CreateClipper(dwFlags, &temp, pUnkOuter);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDClipper = new MyDirectDrawClipperProxy(mLog, temp);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::CreatePalette(DWORD dwFlags, LPPALETTEENTRY lpDDColorArray, LPDIRECTDRAWPALETTE * lplpDDPalette, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    WRITE_FLAG(dwFlags,DDPCAPS_1BIT);
    WRITE_FLAG(dwFlags,DDPCAPS_2BIT);
    WRITE_FLAG(dwFlags,DDPCAPS_4BIT);
    WRITE_FLAG(dwFlags,DDPCAPS_8BIT);
    WRITE_FLAG(dwFlags,DDPCAPS_8BITENTRIES);
    WRITE_FLAG(dwFlags,DDPCAPS_ALPHA);
    WRITE_FLAG(dwFlags,DDPCAPS_ALLOW256);
    WRITE_FLAG(dwFlags,DDPCAPS_INITIALIZE);
    WRITE_FLAG(dwFlags,DDPCAPS_PRIMARYSURFACE);
    WRITE_FLAG(dwFlags,DDPCAPS_PRIMARYSURFACELEFT);
    WRITE_FLAG(dwFlags,DDPCAPS_VSYNC);
    LPDIRECTDRAWPALETTE temp;
    const HRESULT hr = mDD->CreatePalette(dwFlags, lpDDColorArray, &temp, pUnkOuter);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDPalette = new MyDirectDrawPaletteProxy(mLog, temp);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::CreateSurface(LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE * lplpDDSurface, IUnknown * pUnkOuter)
{
    LOG_FUNCTION();
    write_log(lpDDSurfaceDesc, mLog);
    LPDIRECTDRAWSURFACE temp;
    const HRESULT hr = mDD->CreateSurface(lpDDSurfaceDesc, &temp, pUnkOuter);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE * lplpDupDDSurface)
{
    LOG_FUNCTION();
    MyDirectDrawSurfaceProxy* temp = adjust_pointer<MyDirectDrawSurfaceProxy>(lpDDSurface);
    LPDIRECTDRAWSURFACE temp1;
    const HRESULT hr = mDD->DuplicateSurface(temp->origSurf1(), &temp1);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDupDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp1, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags,DDEDM_REFRESHRATES);
    WRITE_FLAG(dwFlags,DDEDM_STANDARDVGAMODES);
    write_log(lpDDSurfaceDesc,mLog);
    DmEnumContext cx(lpContext, this, lpEnumModesCallback);
    return mDD->EnumDisplayModes(dwFlags, lpDDSurfaceDesc, &cx, &MyDirectDrawProxy::EnumModesCallback);
}
HRESULT DD_API MyDirectDrawProxy::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
    //return mDD->EnumSurfaces(dwFlags, lpDDSD, lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawProxy::FlipToGDISurface()
{
    LOG_FUNCTION();
    return mDD->FlipToGDISurface();
}
HRESULT DD_API MyDirectDrawProxy::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDEmulCaps)
{
    LOG_FUNCTION();
    const HRESULT hr = mDD->GetCaps(lpDDDriverCaps, lpDDEmulCaps);
    if(SUCCEEDED(hr))
    {
        LOG_INFO() << "-= Driver Caps =-";
        write_log(lpDDDriverCaps,mLog);
        LOG_INFO() << "-= Emul Caps =-";
        write_log(lpDDEmulCaps,mLog);
    }
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::GetDisplayMode(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return mDD->GetDisplayMode(lpDDSurfaceDesc);
}
HRESULT DD_API MyDirectDrawProxy::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes)
{
    LOG_FUNCTION();
    return mDD->GetFourCCCodes(lpNumCodes, lpCodes);
}
HRESULT DD_API MyDirectDrawProxy::GetGDISurface(LPDIRECTDRAWSURFACE *lplpGDIDDSSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE temp;
    const HRESULT hr = mDD->GetGDISurface(&temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpGDIDDSSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::GetMonitorFrequency(LPDWORD lpdwFrequency)
{
    LOG_FUNCTION();
    return mDD->GetMonitorFrequency(lpdwFrequency);
}
HRESULT DD_API MyDirectDrawProxy::GetScanLine(LPDWORD lpdwScanLine)
{
    LOG_FUNCTION();
    return mDD->GetScanLine(lpdwScanLine);
}
HRESULT DD_API MyDirectDrawProxy::GetVerticalBlankStatus(LPBOOL lpbIsInVB)
{
    LOG_FUNCTION();
    return mDD->GetVerticalBlankStatus(lpbIsInVB);
}
HRESULT DD_API MyDirectDrawProxy::Initialize(GUID* g)
{
    LOG_FUNCTION();
    return mDD->Initialize(g);
}
HRESULT DD_API MyDirectDrawProxy::RestoreDisplayMode()
{
    LOG_FUNCTION();
    return mDD->RestoreDisplayMode();
}
HRESULT DD_API MyDirectDrawProxy::SetCooperativeLevel(HWND hWnd, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(hWnd);
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags,DDSCL_ALLOWMODEX);
    WRITE_FLAG(dwFlags,DDSCL_ALLOWREBOOT);
    WRITE_FLAG(dwFlags,DDSCL_CREATEDEVICEWINDOW);
    WRITE_FLAG(dwFlags,DDSCL_EXCLUSIVE);
    WRITE_FLAG(dwFlags,DDSCL_FPUPRESERVE);
    WRITE_FLAG(dwFlags,DDSCL_FPUSETUP);
    WRITE_FLAG(dwFlags,DDSCL_FULLSCREEN);
    WRITE_FLAG(dwFlags,DDSCL_MULTITHREADED);
    WRITE_FLAG(dwFlags,DDSCL_NORMAL);
    WRITE_FLAG(dwFlags,DDSCL_NOWINDOWCHANGES);
    WRITE_FLAG(dwFlags,DDSCL_SETDEVICEWINDOW);
    WRITE_FLAG(dwFlags,DDSCL_SETFOCUSWINDOW);
    return mDD->SetCooperativeLevel(hWnd, dwFlags);
}
HRESULT DD_API MyDirectDrawProxy::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP)
{
    LOG_FUNCTION();
    WRITE_VAR(dwWidth);
    WRITE_VAR(dwHeight);
    WRITE_VAR(dwBPP);
    return mDD->SetDisplayMode(dwWidth, dwHeight, dwBPP);
}
HRESULT DD_API MyDirectDrawProxy::WaitForVerticalBlank(DWORD dwFlags, HANDLE hEvent)
{
    LOG_FUNCTION();
    return mDD->WaitForVerticalBlank(dwFlags, hEvent);
}

//IDirectDraw2
HRESULT DD_API MyDirectDrawProxy::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwRefreshRate, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwWidth);
    WRITE_VAR(dwHeight);
    WRITE_VAR(dwBPP);
    WRITE_VAR(dwRefreshRate);
    WRITE_VAR(dwFlags);
    return mDD2->SetDisplayMode(dwWidth, dwHeight, dwBPP, dwRefreshRate, dwFlags);
}
HRESULT DD_API MyDirectDrawProxy::GetAvailableVidMem(LPDDSCAPS lpDDSCaps, LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
    LOG_FUNCTION();
    return mDD2->GetAvailableVidMem(lpDDSCaps, lpdwTotal, lpdwFree);
}

//IDirectDraw4
HRESULT DD_API MyDirectDrawProxy::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPDIRECTDRAWSURFACE4 *lplpDDSurface, IUnknown *pUnkOuter)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE4 temp;
    const HRESULT hr = mDD4->CreateSurface(lpDDSurfaceDesc, &temp, pUnkOuter);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::DuplicateSurface(LPDIRECTDRAWSURFACE4 lpDDSurface, LPDIRECTDRAWSURFACE4 *lplpDupDDSurface)
{
    LOG_FUNCTION();
    MyDirectDrawSurfaceProxy* temp = adjust_pointer<MyDirectDrawSurfaceProxy>(lpDDSurface);
    LPDIRECTDRAWSURFACE4 temp1;
    const HRESULT hr = mDD4->DuplicateSurface(temp->origSurf4(), &temp1);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDupDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp1, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK2 lpEnumModesCallback)
{
    LOG_FUNCTION();
    return mDD4->EnumDisplayModes(dwFlags, lpDDSurfaceDesc, lpContext, lpEnumModesCallback);
}
HRESULT DD_API MyDirectDrawProxy::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpEnumSurfacesCallback)
{
    LOG_FUNCTION();
    return mDD4->EnumSurfaces(dwFlags, lpDDSD, lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawProxy::GetDisplayMode(LPDDSURFACEDESC2 lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return mDD4->GetDisplayMode(lpDDSurfaceDesc);
}
HRESULT DD_API MyDirectDrawProxy::GetGDISurface(LPDIRECTDRAWSURFACE4 *lplpGDIDDSSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE4 temp;
    const HRESULT hr = mDD4->GetGDISurface(&temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpGDIDDSSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::GetAvailableVidMem(LPDDSCAPS2 lpDDSCaps, LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
    LOG_FUNCTION();
    return mDD4->GetAvailableVidMem(lpDDSCaps, lpdwTotal, lpdwFree);
}
HRESULT DD_API MyDirectDrawProxy::GetSurfaceFromDC(HDC hdc, LPDIRECTDRAWSURFACE4 *lpDDS)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE4 temp;
    const HRESULT hr = mDD4->GetSurfaceFromDC(hdc, &temp);
    if(FAILED(hr))
    {
        return(hr);
    }
    *lpDDS = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::RestoreAllSurfaces()
{
    LOG_FUNCTION();
    return mDD4->RestoreAllSurfaces();
}
HRESULT DD_API MyDirectDrawProxy::TestCooperativeLevel()
{
    LOG_FUNCTION();
    return mDD4->TestCooperativeLevel();
}
HRESULT DD_API MyDirectDrawProxy::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER lpDDDeviceIdentifier, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDD4->GetDeviceIdentifier(lpDDDeviceIdentifier, dwFlags);
}


//IDirectDraw7
HRESULT DD_API MyDirectDrawProxy::CreateSurface(LPDDSURFACEDESC2 lpDDSurfaceDesc, LPDIRECTDRAWSURFACE7 *lplpDDSurface, IUnknown *pUnkOuter)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE7 temp;
    const HRESULT hr = mDD7->CreateSurface(lpDDSurfaceDesc, &temp, pUnkOuter);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::DuplicateSurface(LPDIRECTDRAWSURFACE7 lpDDSurface, LPDIRECTDRAWSURFACE7 *lplpDupDDSurface)
{
    LOG_FUNCTION();
    MyDirectDrawSurfaceProxy* temp = adjust_pointer<MyDirectDrawSurfaceProxy>(lpDDSurface);
    LPDIRECTDRAWSURFACE7 temp1;
    const HRESULT hr = mDD7->DuplicateSurface(temp->origSurf7(), &temp1);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDupDDSurface = new MyDirectDrawSurfaceProxy(mLog, temp1, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::EnumSurfaces(DWORD dwFlags, LPDDSURFACEDESC2 lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
{
    LOG_FUNCTION();
    return mDD7->EnumSurfaces(dwFlags, lpDDSD, lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawProxy::GetGDISurface(LPDIRECTDRAWSURFACE7 *lplpGDIDDSSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE7 temp;
    const HRESULT hr = mDD7->GetGDISurface(&temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpGDIDDSSurface = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::GetSurfaceFromDC(HDC hdc, LPDIRECTDRAWSURFACE7 *lpDDS)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE7 temp;
    const HRESULT hr = mDD7->GetSurfaceFromDC(hdc, &temp);
    if(FAILED(hr))
    {
        return(hr);
    }
    *lpDDS = new MyDirectDrawSurfaceProxy(mLog, temp, this);
    return hr;
}
HRESULT DD_API MyDirectDrawProxy::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2 lpDDDeviceIdentifier, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDD7->GetDeviceIdentifier(lpDDDeviceIdentifier, dwFlags);
}
HRESULT DD_API MyDirectDrawProxy::StartModeTest(LPSIZE lpModesToTest, DWORD dwNumEntries, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDD7->StartModeTest(lpModesToTest, dwNumEntries, dwFlags);
}
HRESULT DD_API MyDirectDrawProxy::EvaluateMode(DWORD dwFlags, DWORD *pSecondsUntilTimeout)
{
    LOG_FUNCTION();
    return mDD7->EvaluateMode(dwFlags, pSecondsUntilTimeout);
}

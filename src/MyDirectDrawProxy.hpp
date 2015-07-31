#ifndef MYDIRECTDRAW_HPP
#define MYDIRECTDRAW_HPP

#include "common.hpp"

class MyDirectDrawProxy :
        public IDirectDraw,
        public IDirectDraw2,
        public IDirectDraw4,
        public IDirectDraw7
{
    std::atomic<ULONG> mRefCount;
    logger* mLog       = nullptr;
    com_ptr<IDirectDraw>  mDD;
    com_ptr<IDirectDraw2> mDD2;
    com_ptr<IDirectDraw4> mDD4;
    com_ptr<IDirectDraw7> mDD7;
    int mVersion = 1;

    struct DmEnumContext
    {
        void* origContext;
        MyDirectDrawProxy* obj;
        LPDDENUMMODESCALLBACK callback;
        DmEnumContext(void* c, MyDirectDrawProxy* o, LPDDENUMMODESCALLBACK cb):
            origContext(c), obj(o), callback(cb) {}
    };

    static HRESULT WINAPI EnumModesCallback(LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext);
public:
    MyDirectDrawProxy(logger* log, LPDIRECTDRAW dd);
    virtual ~MyDirectDrawProxy();

    HRESULT DD_API QueryInterface(const IID &riid, LPVOID *ppvObj);
    ULONG   DD_API AddRef();
    ULONG   DD_API Release();

    //IDirectDraw
    HRESULT DD_API Compact();
    HRESULT DD_API CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *, IUnknown *);
    HRESULT DD_API CreatePalette(DWORD, LPPALETTEENTRY, LPDIRECTDRAWPALETTE *, IUnknown *);
    HRESULT DD_API CreateSurface(LPDDSURFACEDESC, LPDIRECTDRAWSURFACE *, IUnknown *);
    HRESULT DD_API DuplicateSurface(LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE *);
    HRESULT DD_API EnumDisplayModes(DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMMODESCALLBACK);
    HRESULT DD_API EnumSurfaces(DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK);
    HRESULT DD_API FlipToGDISurface();
    HRESULT DD_API GetCaps(LPDDCAPS, LPDDCAPS);
    HRESULT DD_API GetDisplayMode(LPDDSURFACEDESC);
    HRESULT DD_API GetFourCCCodes(LPDWORD, LPDWORD);
    HRESULT DD_API GetGDISurface(LPDIRECTDRAWSURFACE *);
    HRESULT DD_API GetMonitorFrequency(LPDWORD);
    HRESULT DD_API GetScanLine(LPDWORD);
    HRESULT DD_API GetVerticalBlankStatus(LPBOOL);
    HRESULT DD_API Initialize(GUID *);
    HRESULT DD_API RestoreDisplayMode();
    HRESULT DD_API SetCooperativeLevel(HWND, DWORD);
    HRESULT DD_API SetDisplayMode(DWORD, DWORD, DWORD);
    HRESULT DD_API WaitForVerticalBlank(DWORD, HANDLE);

    //IDirectDraw2
    HRESULT DD_API SetDisplayMode(DWORD, DWORD, DWORD, DWORD, DWORD);
    HRESULT DD_API GetAvailableVidMem(LPDDSCAPS, LPDWORD, LPDWORD);

    //IDirectDraw4
    HRESULT DD_API CreateSurface(LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE4 *, IUnknown *);
    HRESULT DD_API DuplicateSurface(LPDIRECTDRAWSURFACE4, LPDIRECTDRAWSURFACE4 *);
    HRESULT DD_API EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMMODESCALLBACK2);
    HRESULT DD_API EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK2);
    HRESULT DD_API GetDisplayMode(LPDDSURFACEDESC2);
    HRESULT DD_API GetGDISurface(LPDIRECTDRAWSURFACE4 *);
    HRESULT DD_API GetAvailableVidMem(LPDDSCAPS2, LPDWORD, LPDWORD);
    HRESULT DD_API GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE4 *);
    HRESULT DD_API RestoreAllSurfaces();
    HRESULT DD_API TestCooperativeLevel();
    HRESULT DD_API GetDeviceIdentifier(LPDDDEVICEIDENTIFIER, DWORD);

    //IDirectDraw7
    HRESULT DD_API CreateSurface(LPDDSURFACEDESC2, LPDIRECTDRAWSURFACE7 *, IUnknown *);
    HRESULT DD_API DuplicateSurface(LPDIRECTDRAWSURFACE7, LPDIRECTDRAWSURFACE7 *);
    HRESULT DD_API EnumSurfaces(DWORD, LPDDSURFACEDESC2, LPVOID, LPDDENUMSURFACESCALLBACK7);
    HRESULT DD_API GetGDISurface(LPDIRECTDRAWSURFACE7 *);
    HRESULT DD_API GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE7 *);
    HRESULT DD_API GetDeviceIdentifier(LPDDDEVICEIDENTIFIER2, DWORD);
    HRESULT DD_API StartModeTest(LPSIZE, DWORD, DWORD);
    HRESULT DD_API EvaluateMode(DWORD, DWORD *);
};

#endif // MYDIRECTDRAW_HPP

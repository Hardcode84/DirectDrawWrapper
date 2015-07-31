#ifndef MYDIRECTDRAWSW_HPP
#define MYDIRECTDRAWSW_HPP

#include <set>
#include <chrono>

#include "common.hpp"
#include "common_structs.hpp"
#include "tools/window_hook.hpp"

#include "MyDirectDrawSurfaceSw.hpp"

class Settings;

class MyDirectDrawPaletteSw;

class MyDirectDrawSw :
        public IDirectDraw,
        public IDirectDraw2,
        public IDirectDraw4,
        public IDirectDraw7
{
    typedef std::chrono::high_resolution_clock clock;
    typedef std::chrono::microseconds microseconds;
    typedef std::chrono::seconds seconds;

    std::atomic<ULONG> mRefCount;
    logger* mLog       = nullptr;
    int mVersion = 1;
    DisplayMode mDispMode;
    HWND mHWnd = nullptr;
    LONG_PTR mWndStyle = 0;
    HDC mHDC = nullptr;
    bool mIsFullscreen = false;
    bool mIsModeChanged = false;
    DEVMODE mSavedDispMode;
    MyDirectDrawSurfaceSw* mPrimarySurface = nullptr;
    const Settings* mSettings = nullptr;
    window_hook mHook;
    UINT_PTR mUpdateTimer;

    std::set<MyDirectDrawSurfaceSw*> mSurfaces;

    clock::time_point mPrevFrameTime;
    int mFramesCount = 0;
    float mFrameTime = 0.0f;

    MyDirectDrawSw(logger* log, const Settings* settings);
    virtual ~MyDirectDrawSw();

    static MyDirectDrawSw* instance;
public:
    static MyDirectDrawSw* getInstance(logger* log, const Settings* settings);
    static MyDirectDrawSw* getInstance();
    static void ReleaseInstance();

    void setPrimarySurf(MyDirectDrawSurfaceSw* surf) { mPrimarySurface = surf; }
    MyDirectDrawSurfaceSw* getPrimarySurf() const { return mPrimarySurface; }

    logger* getLogger()  const { return mLog; }
    float getFrameTime() const { return mFrameTime; }
    float getFPS()       const { return 1.0f / mFrameTime; }
    HWND  getHWnd()      const { return mHWnd; }

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

public:
    const DisplayMode& getDisplayMode() const { return mDispMode; }
    bool setWindow(HWND hWnd, bool fullscreen);
    bool setDisplayModeInternal(const DisplayMode& mode);
    void restoreDisplayModeInternal();
    bool presentPrimarySurfInternal();
    bool presentInternal(int xdest,
                         int ydest,
                         int width,
                         int height,
                         int xsrc,
                         int ysrc,
                         const MyDirectDrawSurfaceSw* surf);
    bool setClientRect(HWND hWnd, int w, int h) const;
    bool initWindow();
    HDC getDC() const { return mHDC; }
    std::pair<int,int> getWindowPos(bool real = false) const;
    bool isFullscreen() const { return mIsFullscreen; }

    void getDescInternal(LPDDCAPS caps) const;
    void getCapsInternal(DWORD* dwCaps) const;
    void getCaps2Internal(DWORD* dwCaps2) const;
    void getCKeyCapsInternal(DWORD* dwCKeyCaps) const;
    void getFxCapsInternal(DWORD* dwFXCaps) const;
    void getDDSCapsInternal1(DWORD* dwCaps) const;

    void enumDispModesInternal(void* context, LPDDENUMMODESCALLBACK callback);

    MyDirectDrawSurfaceSw* createSurfaceInternal(const LPDDSURFACEDESC desc);

    void registerSurfInternal(MyDirectDrawSurfaceSw* s);
    void unregisterSurfInternal(MyDirectDrawSurfaceSw* s);
    std::string getDebugInfo() const;
private:
    LRESULT myWndProcInternal(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam, WNDPROC original);
    void clearWindowDataInternal();
    bool needWindowHook() const;
};

#endif // MYDIRECTDRAWSW_HPP

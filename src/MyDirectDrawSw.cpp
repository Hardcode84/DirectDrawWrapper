#include "MyDirectDrawSw.hpp"

#include "MyDirectDrawPaletteSw.hpp"
#include "MyWinapiFuncs.hpp"
#include "serialize_helpers.hpp"
#include "Settings.hpp"

MyDirectDrawSw* MyDirectDrawSw::instance = nullptr;

MyDirectDrawSw::MyDirectDrawSw(logger *log, const Settings *settings):
    mRefCount(1),
    mLog(log),
    mSettings(settings),
    mHook(log, std::bind(&MyDirectDrawSw::myWndProcInternal, this,
                         std::placeholders::_1,
                         std::placeholders::_2,
                         std::placeholders::_3,
                         std::placeholders::_4,
                         std::placeholders::_5))
{
    LOG_FUNCTION();
    mPrevFrameTime = clock::now();
}

MyDirectDrawSw::~MyDirectDrawSw()
{
    LOG_FUNCTION();
    if(mHDC)
    {
        ReleaseDC(mHWnd, mHDC);
    }
}

MyDirectDrawSw* MyDirectDrawSw::getInstance(logger* log, const Settings* settings)
{
    if(nullptr == instance)
    {
        instance = new MyDirectDrawSw(log, settings);
    }
    return instance;
}

MyDirectDrawSw* MyDirectDrawSw::getInstance()
{
    return instance;
}

void MyDirectDrawSw::ReleaseInstance()
{
    if(nullptr != instance)
    {
        instance->Release();
        instance = nullptr;
    }
}

DEFINE_GUID( IID_IDirect3DNullDevice, 0x8767df22, 0xbacc, 0x11d1, 0x89, 0x69, 0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);
DEFINE_GUID( IID_IDirect3D7,          0xf5049e77, 0x4861, 0x11d2, 0xa4, 0x7,  0x0, 0xa0, 0xc9, 0x6, 0x29, 0xa8);

HRESULT DD_API MyDirectDrawSw::QueryInterface(const IID &riid, LPVOID *ppvObj)
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
        LOG_INFO() << "Unknown DD version. IID: " << riid;
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
        *ppvObj = static_cast<LPDIRECTDRAW2>(this);
    }
    if(mVersion >= 4)
    {
        *ppvObj = static_cast<LPDIRECTDRAW4>(this);
    }
    if(mVersion >= 7)
    {
        *ppvObj = static_cast<LPDIRECTDRAW7>(this);
    }
    return S_OK;
}

ULONG   DD_API MyDirectDrawSw::AddRef()
{
    return (ULONG)(++mRefCount) - 1; //one instance in sigleton
}

ULONG   DD_API MyDirectDrawSw::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret - 1;
}

bool MyDirectDrawSw::setWindow(HWND hWnd, bool fullscreen)
{
    clearWindowDataInternal();
    if(mSettings->hookGetCursorPos())
    {
        if(!CreateHooks(mLog))
        {
            LOG_ERROR() << "Unable to set hooks";
            return false;
        }
    }
    mHWnd = hWnd;
    if(mHWnd)
    {
        if(needWindowHook())
        {
            if(!mHook.hookWindow(mHWnd))
            {
                mHWnd = nullptr;
                return false;
            }
            const auto updateInterval = mSettings->forceUpdateMsec();
            if(updateInterval >= 0)
            {
                const UINT_PTR newTimer = 123456 + rand();
                if(0 == ::SetTimer(mHWnd, newTimer, updateInterval, nullptr))
                {
                    clearWindowDataInternal();
                    LOG_ERROR() << "SetTimer failed: " << getWinError();
                    return false;
                }
                mUpdateTimer = newTimer;
            }
        }
        mWndStyle = ::GetWindowLongPtr(mHWnd, GWL_STYLE);
        RECT rc;
        if(::GetClientRect(mHWnd, &rc))
        {
            mDispMode.width  = rc.right  - rc.left;
            mDispMode.height = rc.bottom - rc.top;
        }
    }
    else
    {
        mWndStyle = 0;
    }
    mIsFullscreen = fullscreen;
    return initWindow();
}

bool MyDirectDrawSw::setDisplayModeInternal(const DisplayMode& mode)
{
    if(nullptr == mHWnd)
    {
        LOG_ERROR() << "Window not set";
        return false;
    }
    restoreDisplayModeInternal();
    mDispMode = mode;
    if(0 == mDispMode.bpp)
    {
        mDispMode.bpp = 32;
    }

    /*
    if(mHWnd && mIsFullscreen)
    {
        if(!EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &mSavedDispMode))
        {
            LOG_ERROR() << "EnumDisplaySettings failed: " << getWinError();
            return false;
        }
        DEVMODE newMode = mSavedDispMode;
        newMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        newMode.dmBitsPerPel = 32;
        newMode.dmPelsWidth = mDispMode.width;
        newMode.dmPelsHeight  = mDispMode.height;
        if(mDispMode.refreshRate > 0)
        {
            newMode.dmFields |= DM_DISPLAYFREQUENCY;
            newMode.dmDisplayFrequency = mDispMode.refreshRate;
        }
        if(DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&newMode, CDS_FULLSCREEN))
        {
            LOG_ERROR() << "ChangeDisplaySettings failed: " << getWinError();
            return false;
        }
        mIsModeChanged = true;
    }
    */
    return initWindow();
}

void MyDirectDrawSw::restoreDisplayModeInternal()
{
    if(mIsModeChanged)
    {
        mIsModeChanged = false;
        /*
        if(DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(nullptr, 0))
        {
            LOG_ERROR() << "ChangeDisplaySettings failed: " << getWinError();
        }
        */
    }
}

bool MyDirectDrawSw::presentPrimarySurfInternal()
{
    if(nullptr == mPrimarySurface)
    {
        LOG_ERROR() << "Primary surface is null";
        return false;
    }
    const auto w = mPrimarySurface->width();
    const auto h = mPrimarySurface->height();
    return presentInternal(0, 0, w, h, 0, 0, mPrimarySurface);
}

bool MyDirectDrawSw::presentInternal(int xdest,
                                     int ydest,
                                     int width,
                                     int height,
                                     int xsrc,
                                     int ysrc,
                                     const MyDirectDrawSurfaceSw* surf)
{
    LOG_FUNCTION();
    ++mFramesCount;
    const auto newTime = clock::now();
    if(std::chrono::duration_cast<seconds>(newTime - mPrevFrameTime).count() >= 1)
    {
        const auto dur = std::chrono::duration_cast<microseconds>(newTime - mPrevFrameTime);
        mFrameTime = (decltype(mFrameTime))dur.count() /
                (decltype(mFrameTime))mFramesCount / 1000000.0f;
        mFramesCount = 0;
        mPrevFrameTime = newTime;
    }

    int xoff = 0;
    int yoff = 0;
    if(mSettings->drawOnScreen())
    {
        const auto off = getWindowPos(true);
        xoff = off.first;
        yoff = off.second;
    }
    xdest += xoff;
    ydest += yoff;
    const auto bpp = surf->bpp();
    const void* data = surf->GetBufferPtr();

    struct
    {
        BITMAPINFO bmi;
        RGBQUAD data[255];
    } s;
    s.bmi.bmiHeader.biSize = sizeof(s.bmi.bmiHeader);
    s.bmi.bmiHeader.biWidth = width;
    s.bmi.bmiHeader.biHeight = -height;
    s.bmi.bmiHeader.biPlanes = 1;
    s.bmi.bmiHeader.biBitCount = bpp;
    s.bmi.bmiHeader.biCompression = BI_RGB;
    s.bmi.bmiHeader.biSizeImage = 0;
    s.bmi.bmiHeader.biXPelsPerMeter = 0;
    s.bmi.bmiHeader.biYPelsPerMeter = 0;
    s.bmi.bmiHeader.biClrUsed = 0;
    s.bmi.bmiHeader.biClrImportant = 0;
    if(bpp <= 8)
    {
        const auto palette = surf->getPaletteInternal();
        if(nullptr != palette)
        {
            int i = 0;
            std::generate_n(s.bmi.bmiColors, palette->count(),[&]()
            {
                const auto& e = palette->entry(i++);
                RGBQUAD ret;
                ret.rgbBlue  = e.peBlue;
                ret.rgbGreen = e.peGreen,
                ret.rgbRed   = e.peRed;
                return ret;
            });
        }
        else
        {
            memset(&s.bmi.bmiColors, 0, sizeof(RGBQUAD) * 256);
        }
    }
    else if(bpp < 32)
    {
        s.bmi.bmiHeader.biCompression = BI_BITFIELDS;
        auto masks = reinterpret_cast<DWORD*>(s.bmi.bmiColors);
        surf->getColorMask(masks[0], masks[1], masks[2]);
    }

    const bool result = (0 != ::SetDIBitsToDevice(mHDC,
                                                  xdest,
                                                  ydest,
                                                  width,
                                                  height,
                                                  xsrc,
                                                  ysrc,
                                                  0,
                                                  height,
                                                  data,
                                                  &s.bmi,
                                                  DIB_RGB_COLORS));
    if(!result)
    {
        LOG_ERROR() << "SetDIBitsToDevice failed: " << getWinError();
    }

    if(mSettings->showDebugInfo())
    {
        RECT textRc = {xoff,yoff,mDispMode.width + xoff,mDispMode.height + yoff};
        ::DrawText(mHDC, getDebugInfo().c_str(), -1, &textRc, DT_LEFT | DT_TOP);
    }

    return result;
}

bool MyDirectDrawSw::setClientRect(HWND hWnd, int w, int h) const
{
    RECT rect = {0,0,w,h};
    RECT rect2;
    if(!::AdjustWindowRectEx(&rect,
                             ::GetWindowLong(hWnd,GWL_STYLE),
                             (BOOL)::GetMenu(hWnd),
                             ::GetWindowLong(hWnd, GWL_EXSTYLE)))
    {
        LOG_ERROR() << "AdjustWindowRectEx failed: " << getWinError();
        return false;
    }
    if(!::GetWindowRect(hWnd, &rect2))
    {
        LOG_ERROR() << "GetWindowRect failed: " << getWinError();
        return false;
    }
    if(!::MoveWindow(hWnd, rect2.left, rect2.top, rect.right-rect.left,rect.bottom-rect.top, TRUE))
    {
        LOG_ERROR() << "MoveWindow failed: " << getWinError();
        return false;
    }
    return true;
}

bool MyDirectDrawSw::initWindow()
{
    if(mHWnd)
    {
        if(!mHDC)
        {
            if(mSettings->drawOnScreen())
            {
                mHDC = ::GetDC(nullptr);
            }
            else
            {
                mHDC = ::GetDC(mHWnd);
            }

            if(nullptr == mHDC)
            {
                LOG_ERROR() << "GetDC failed: " << getWinError();
                return false;
            }
        }

        if(mIsFullscreen)
        {
            if(mSettings->addWindowFrame())
            {
                const LONG_PTR newStyle = (mWndStyle |= WS_CAPTION);
                ::SetWindowLongPtr(mHWnd, GWL_STYLE, newStyle);
            }
            return setClientRect(mHWnd, mDispMode.width, mDispMode.height);
        }
    }
    return true;
}

std::pair<int,int> MyDirectDrawSw::getWindowPos(bool real) const
{
    if(nullptr == mHWnd || (!real && mIsFullscreen))
    {
        return std::make_pair(0,0);
    }
    POINT pt = {0,0};
    ::ClientToScreen(mHWnd, &pt);
    return std::make_pair(pt.x, pt.y);
}

void MyDirectDrawSw::getDescInternal(LPDDCAPS caps) const
{
    if(sizeof(DDCAPS_DX1) == caps->dwSize)
    {
        DDCAPS_DX1* caps1 = reinterpret_cast<DDCAPS_DX1*>(caps);
        memset(caps1, 0, sizeof(DDCAPS_DX1));
        caps1->dwSize = sizeof(DDCAPS_DX1);
        getCapsInternal(&caps1->dwCaps);
        getCaps2Internal(&caps1->dwCaps2);
        getCKeyCapsInternal(&caps1->dwCKeyCaps);
        getFxCapsInternal(&caps1->dwFXCaps);
        getDDSCapsInternal1(&caps1->ddsCaps.dwCaps);
        caps1->dwVidMemTotal = 1024 * 1024 * 1024;
        caps1->dwVidMemFree  = 1024 * 1024 * 1024;
    }
    else if(sizeof(DDCAPS_DX3) == caps->dwSize)
    {
        DDCAPS_DX3* caps3 = reinterpret_cast<DDCAPS_DX3*>(caps);
        memset(caps3, 0, sizeof(DDCAPS_DX3));
        caps3->dwSize = sizeof(DDCAPS_DX3);
        getCapsInternal(&caps3->dwCaps);
        getCaps2Internal(&caps3->dwCaps2);
        getCKeyCapsInternal(&caps3->dwCKeyCaps);
        getFxCapsInternal(&caps3->dwFXCaps);
        getDDSCapsInternal1(&caps3->ddsCaps.dwCaps);
        caps3->dwVidMemTotal = 1024 * 1024 * 1024;
        caps3->dwVidMemFree  = 1024 * 1024 * 1024;
    }
    else if(sizeof(DDCAPS_DX5) == caps->dwSize)
    {
        DDCAPS_DX5* caps5 = reinterpret_cast<DDCAPS_DX5*>(caps);
        memset(caps5, 0, sizeof(DDCAPS_DX5));
        caps5->dwSize = sizeof(DDCAPS_DX5);
        getCapsInternal(&caps5->dwCaps);
        getCaps2Internal(&caps5->dwCaps2);
        getCKeyCapsInternal(&caps5->dwCKeyCaps);
        getFxCapsInternal(&caps5->dwFXCaps);
        getDDSCapsInternal1(&caps5->ddsCaps.dwCaps);
        caps5->dwVidMemTotal = 1024 * 1024 * 1024;
        caps5->dwVidMemFree  = 1024 * 1024 * 1024;
    }
    else if(sizeof(DDCAPS_DX7) == caps->dwSize)
    {
        DDCAPS_DX7* caps7 = reinterpret_cast<DDCAPS_DX7*>(caps);
        memset(caps7, 0, sizeof(DDCAPS_DX7));
        caps7->dwSize = sizeof(DDCAPS_DX7);
        getCapsInternal(&caps7->dwCaps);
        getCaps2Internal(&caps7->dwCaps2);
        getCKeyCapsInternal(&caps7->dwCKeyCaps);
        getFxCapsInternal(&caps7->dwFXCaps);
        getDDSCapsInternal1(&caps7->ddsCaps.dwCaps);
        caps7->dwVidMemTotal = 1024 * 1024 * 1024;
        caps7->dwVidMemFree  = 1024 * 1024 * 1024;
    }
}

void MyDirectDrawSw::getCapsInternal(DWORD* dwCaps) const
{
    *dwCaps |= DDCAPS_BLT;
    *dwCaps |= DDCAPS_BLTCOLORFILL;
    *dwCaps |= DDCAPS_BLTQUEUE;
    *dwCaps |= DDCAPS_BLTSTRETCH;
    *dwCaps |= DDCAPS_CANBLTSYSMEM;
    *dwCaps |= DDCAPS_COLORKEY;
    *dwCaps |= DDCAPS_GDI;
}
void MyDirectDrawSw::getCaps2Internal(DWORD* dwCaps2) const
{
    *dwCaps2 |= DDCAPS2_WIDESURFACES;
}

void MyDirectDrawSw::getCKeyCapsInternal(DWORD* dwCKeyCaps) const
{
    *dwCKeyCaps |= DDCKEYCAPS_SRCBLT;
}

void MyDirectDrawSw::getFxCapsInternal(DWORD* dwFXCaps) const
{
    *dwFXCaps |= DDFXCAPS_BLTFILTER;
    *dwFXCaps |= DDFXCAPS_BLTMIRRORLEFTRIGHT;
    *dwFXCaps |= DDFXCAPS_BLTMIRRORUPDOWN;
    *dwFXCaps |= DDFXCAPS_BLTSHRINKX;
    *dwFXCaps |= DDFXCAPS_BLTSHRINKY;
    *dwFXCaps |= DDFXCAPS_BLTSTRETCHX;
    *dwFXCaps |= DDFXCAPS_BLTSTRETCHY;
}

void MyDirectDrawSw::getDDSCapsInternal1(DWORD* dwCaps) const
{
    *dwCaps |= DDSCAPS_BACKBUFFER;
    *dwCaps |= DDSCAPS_COMPLEX;
    *dwCaps |= DDSCAPS_FLIP;
    *dwCaps |= DDSCAPS_FRONTBUFFER;
    *dwCaps |= DDSCAPS_OFFSCREENPLAIN;
    *dwCaps |= DDSCAPS_PRIMARYSURFACE;
    *dwCaps |= DDSCAPS_VIDEOMEMORY;
}

void MyDirectDrawSw::enumDispModesInternal(void* context, LPDDENUMMODESCALLBACK callback)
{
    DEVMODE mode;
    for(int i = 0;; ++i)
    {
        memset(&mode, 0, sizeof(mode));
        mode.dmSize = sizeof(mode);
        if(!::EnumDisplaySettings(nullptr, i, &mode))
        {
            break;
        }

        DDSURFACEDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize = sizeof(desc);
        desc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_PITCH /*| DDSD_REFRESHRATE*/;
        desc.dwWidth  = mode.dmPelsWidth;
        desc.dwHeight = mode.dmPelsHeight;
        desc.lPitch = desc.dwWidth * (mode.dmBitsPerPel / 8);
        desc.ddpfPixelFormat.dwSize = sizeof(desc.ddpfPixelFormat);
        desc.ddpfPixelFormat.dwRGBBitCount = mode.dmBitsPerPel;
        if(8 == mode.dmBitsPerPel)
        {
            desc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
            desc.ddpfPixelFormat.dwRBitMask = 0;
            desc.ddpfPixelFormat.dwGBitMask = 0;
            desc.ddpfPixelFormat.dwBBitMask = 0;
        }
        else if(16 == mode.dmBitsPerPel)
        {
            desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
            desc.ddpfPixelFormat.dwRBitMask = 0x0000f800;
            desc.ddpfPixelFormat.dwGBitMask = 0x000007e0;
            desc.ddpfPixelFormat.dwBBitMask = 0x0000001f;
        }
        else if(32 == mode.dmBitsPerPel)
        {
            desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
            desc.ddpfPixelFormat.dwRBitMask = 0x00ff0000;
            desc.ddpfPixelFormat.dwGBitMask = 0x0000ff00;
            desc.ddpfPixelFormat.dwBBitMask = 0x000000ff;
        }
        else
        {
            continue;
        }

        if(DDENUMRET_OK != callback(&desc, context))
        {
            break;
        }
        //write_log(&desc, mLog);
    }
}

MyDirectDrawSurfaceSw* MyDirectDrawSw::createSurfaceInternal(const LPDDSURFACEDESC desc)
{
    const bool isPrimary = (desc->dwFlags & DDSD_CAPS) && (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE);
    if(nullptr != getPrimarySurf() && isPrimary)
    {
        LOG_ERROR() << "Attempt to create second primary surface";
        return nullptr;
    }
    const auto ret = MyDirectDrawSurfaceSw::create(desc, mLog, this);
    if(isPrimary)
    {
        setPrimarySurf(ret);
    }
    return ret;
}

void MyDirectDrawSw::registerSurfInternal(MyDirectDrawSurfaceSw* s)
{
    mSurfaces.insert(s);
}

void MyDirectDrawSw::unregisterSurfInternal(MyDirectDrawSurfaceSw* s)
{
    mSurfaces.erase(s);
}

std::string MyDirectDrawSw::getDebugInfo() const
{
    const int BuffSize = 256;
    char buff[BuffSize];
    std::snprintf(buff, BuffSize, "FPS %f, frametime %f\nMode %dx%dx%d, num surfaces: %d",
                  getFPS(),
                  getFrameTime(),
                  mDispMode.width,
                  mDispMode.height,
                  mDispMode.bpp,
                  mSurfaces.size());
    return buff;
}

LRESULT MyDirectDrawSw::myWndProcInternal(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam,
                                          WNDPROC original)
{
    if(WM_TIMER == uMsg && mUpdateTimer == wParam && (mSettings->forceUpdateMsec() >= 0))
    {
        presentPrimarySurfInternal();
        return 0;
    }
    return original(hwnd, uMsg, wParam, lParam);
}

void MyDirectDrawSw::clearWindowDataInternal()
{
    if(mHDC)
    {
        if(mSettings->drawOnScreen())
        {
            ::ReleaseDC(nullptr, mHDC);
        }
        else
        {
            ::ReleaseDC(mHWnd, mHDC);
        }
    }
    if(0 != mUpdateTimer)
    {
        ::KillTimer(mHWnd, mUpdateTimer);
    }
    mHook.unhookWindow();
    mHDC = nullptr;
    if(0 != mUpdateTimer)
    mHWnd = nullptr;
}

bool MyDirectDrawSw::needWindowHook() const
{
    return (mSettings->forceUpdateMsec() >= 0);
}

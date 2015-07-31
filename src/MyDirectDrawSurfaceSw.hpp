#ifndef MYDIRECTDRAWSURFACESW_HPP
#define MYDIRECTDRAWSURFACESW_HPP

#include <memory>
#include <deque>

#include "tools/handle_ptr.hpp"
#include "common.hpp"
#include "MyDirectDrawPaletteSw.hpp"
#include "MyDirectDrawClipperSw.hpp"

class MyDirectDrawSw;

class MyDirectDrawSurfaceSw :
        public IDirectDrawSurface,
        public IDirectDrawSurface2,
        public IDirectDrawSurface3,
        public IDirectDrawSurface4,
        public IDirectDrawSurface7
{
    int mVersion = 1;
    std::atomic<ULONG> mRefCount;
    logger* mLog            = nullptr;
    MyDirectDrawSw* mParent = nullptr;

    com_ptr<MyDirectDrawPaletteSw> mPalette;
    com_ptr<MyDirectDrawClipperSw> mClipper;

    std::deque<com_ptr<MyDirectDrawSurfaceSw> > mChain;
    int mWidth  = 0;
    int mHeight = 0;
    int mBpp    = 0;

    std::unique_ptr<unsigned char[]> mBuffer;

    bool mIsFrontBuffer = false;
    bool mIsBackBuffer  = false;
    bool mIsOffScrPlain = false;

    int mLockCount = 0;
    int mPageLockCount = 0;
    int mDCCount = 0;

    gdi_handle_ptr<HBITMAP> mHBitmap;
    HDC mHDC = nullptr;
    void* mBitmapData = nullptr;

    DDCOLORKEY mDstColorKey;
    DDCOLORKEY mSrcColorKey;

    int mPaletteStamp = 0;

    template<typename SrcT>
    static MyDirectDrawSurfaceSw* adjPtr(SrcT* src)
    {
        return adjust_pointer<MyDirectDrawSurfaceSw>(src);
    }

    // Some games like Diablo I writes outside the buffer
    static const size_t BufferPadding = 1024 * 10;

    MyDirectDrawSurfaceSw(logger* log, MyDirectDrawSw* parent,
                          int width,
                          int height,
                          int bpp,
                          bool plain);
    virtual ~MyDirectDrawSurfaceSw();
public:
    static MyDirectDrawSurfaceSw* create(LPDDSURFACEDESC desc, logger* log, MyDirectDrawSw* parent);

    bool isLocked()     const { return (mLockCount > 0); }
    bool isPageLocked() const { return (mPageLockCount > 0); }
    bool isDCOut()      const { return (mDCCount > 0); }
    bool isSmthLocked() const { return isLocked() || isPageLocked() || isDCOut(); }


    bool isPaletted()   const { return (8 == mBpp); }
    HRESULT BltInternal(LPRECT lpDestRect,
                        MyDirectDrawSurfaceSw* lpDDSrcSurface,
                        LPRECT lpSrcRect,
                        DWORD dwFlags,
                        LPDDBLTFX lpDDBltFx);
    HRESULT BltFastInternal(DWORD dwX,
                            DWORD dwY,
                            MyDirectDrawSurfaceSw* lpDDSrcSurface,
                            LPRECT lpSrcRect,
                            DWORD dwFlags);
    HRESULT FlipInternal(MyDirectDrawSurfaceSw* targetOverride, DWORD dwFlags);
    void GetPixelFormatInternal(LPDDPIXELFORMAT pf) const;
    void GetCapsInternal(DDSCAPS* caps) const;
    void GetDescInternal(LPDDSURFACEDESC desc) const;
    MyDirectDrawSurfaceSw* GetAttachedSurfaceInternal(const LPDDSCAPS caps);
    size_t pitch() const { return mWidth * (mBpp / 8); }
    size_t bufferSize() const { return mWidth * mHeight * (mBpp / 8); }
    int width()  const { return mWidth; }
    int height() const { return mHeight; }
    int bpp()    const { return mBpp; }
    void getColorMask(DWORD& red, DWORD& green, DWORD& blue) const;

    void OnChangedInternal();

    unsigned char* GetBufferPtr() const { return mBuffer.get() + BufferPadding; }
    MyDirectDrawPaletteSw* getPaletteInternal() const;

    HRESULT LockInternal(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags);
    HRESULT UnlockInternal();

    HRESULT GetDCInternal(HDC* hdc);
    HRESULT ReleaseDCInternal(HDC hdc);

    HRESULT CreateBitmapInternal();
    HRESULT FlushBitmapInternal();
    HRESULT ReleaseBitmapInternal();

    HRESULT DD_API QueryInterface(const IID &riid, LPVOID *ppvObj);
    ULONG   DD_API AddRef();
    ULONG   DD_API Release();

    //IDirectDrawSurface
    HRESULT DD_API AddAttachedSurface(LPDIRECTDRAWSURFACE);
    HRESULT DD_API AddOverlayDirtyRect(LPRECT);
    HRESULT DD_API Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX);
    HRESULT DD_API BltBatch(LPDDBLTBATCH, DWORD, DWORD);
    HRESULT DD_API BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT, DWORD);
    HRESULT DD_API DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE);
    HRESULT DD_API EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK);
    HRESULT DD_API EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK);
    HRESULT DD_API Flip(LPDIRECTDRAWSURFACE, DWORD);
    HRESULT DD_API GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE *);
    HRESULT DD_API GetBltStatus(DWORD);
    HRESULT DD_API GetCaps(LPDDSCAPS);
    HRESULT DD_API GetClipper(LPDIRECTDRAWCLIPPER *);
    HRESULT DD_API GetColorKey(DWORD, LPDDCOLORKEY);
    HRESULT DD_API GetDC(HDC *);
    HRESULT DD_API GetFlipStatus(DWORD);
    HRESULT DD_API GetOverlayPosition(LPLONG, LPLONG);
    HRESULT DD_API GetPalette(LPDIRECTDRAWPALETTE *);
    HRESULT DD_API GetPixelFormat(LPDDPIXELFORMAT);
    HRESULT DD_API GetSurfaceDesc(LPDDSURFACEDESC);
    HRESULT DD_API Initialize(LPDIRECTDRAW, LPDDSURFACEDESC);
    HRESULT DD_API IsLost();
    HRESULT DD_API Lock(LPRECT, LPDDSURFACEDESC, DWORD, HANDLE);
    HRESULT DD_API ReleaseDC(HDC);
    HRESULT DD_API Restore();
    HRESULT DD_API SetClipper(LPDIRECTDRAWCLIPPER);
    HRESULT DD_API SetColorKey(DWORD, LPDDCOLORKEY);
    HRESULT DD_API SetOverlayPosition(LONG, LONG);
    HRESULT DD_API SetPalette(LPDIRECTDRAWPALETTE);
    HRESULT DD_API Unlock(LPVOID);
    HRESULT DD_API UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX);
    HRESULT DD_API UpdateOverlayDisplay(DWORD);
    HRESULT DD_API UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE);

    //IDirectDrawSurface2
    HRESULT DD_API AddAttachedSurface(LPDIRECTDRAWSURFACE2);
    HRESULT DD_API Blt(LPRECT, LPDIRECTDRAWSURFACE2, LPRECT, DWORD, LPDDBLTFX);
    HRESULT DD_API BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE2, LPRECT, DWORD);
    HRESULT DD_API DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE2);
    HRESULT DD_API Flip(LPDIRECTDRAWSURFACE2, DWORD);
    HRESULT DD_API GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE2 *);
    HRESULT DD_API UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE2, LPRECT, DWORD, LPDDOVERLAYFX);
    HRESULT DD_API UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE2);
    HRESULT DD_API GetDDInterface(LPVOID *);
    HRESULT DD_API PageLock(DWORD);
    HRESULT DD_API PageUnlock(DWORD);

    //IDirectDrawSurface3
    HRESULT DD_API AddAttachedSurface(LPDIRECTDRAWSURFACE3);
    HRESULT DD_API Blt(LPRECT, LPDIRECTDRAWSURFACE3, LPRECT, DWORD, LPDDBLTFX);
    HRESULT DD_API BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE3, LPRECT, DWORD);
    HRESULT DD_API DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE3);
    HRESULT DD_API Flip(LPDIRECTDRAWSURFACE3, DWORD);
    HRESULT DD_API GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE3 *);
    HRESULT DD_API UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE3, LPRECT, DWORD, LPDDOVERLAYFX);
    HRESULT DD_API UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE3);
    HRESULT DD_API SetSurfaceDesc(LPDDSURFACEDESC, DWORD);

    //IDirectDrawSurface4
    HRESULT DD_API AddAttachedSurface(LPDIRECTDRAWSURFACE4);
    HRESULT DD_API Blt(LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDBLTFX);
    HRESULT DD_API BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE4, LPRECT, DWORD);
    HRESULT DD_API DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE4);
    HRESULT DD_API EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK2);
    HRESULT DD_API EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK2);
    HRESULT DD_API Flip(LPDIRECTDRAWSURFACE4, DWORD);
    HRESULT DD_API FreePrivateData(const GUID &);
    HRESULT DD_API GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE4 *);
    HRESULT DD_API GetCaps(LPDDSCAPS2);
    HRESULT DD_API GetSurfaceDesc(LPDDSURFACEDESC2);
    HRESULT DD_API Initialize(LPDIRECTDRAW, LPDDSURFACEDESC2);
    HRESULT DD_API Lock(LPRECT, LPDDSURFACEDESC2, DWORD, HANDLE);
    HRESULT DD_API Unlock(LPRECT);
    HRESULT DD_API UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE4, LPRECT, DWORD, LPDDOVERLAYFX);
    HRESULT DD_API UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE4);
    HRESULT DD_API SetSurfaceDesc(LPDDSURFACEDESC2, DWORD);
    HRESULT DD_API SetPrivateData(const GUID &, LPVOID, DWORD, DWORD);
    HRESULT DD_API GetPrivateData(const GUID &, LPVOID, LPDWORD);
    HRESULT DD_API GetUniquenessValue(LPDWORD);
    HRESULT DD_API ChangeUniquenessValue();

    //IDirectDrawSurface7
    HRESULT DD_API AddAttachedSurface(LPDIRECTDRAWSURFACE7);
    HRESULT DD_API Blt(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDBLTFX);
    HRESULT DD_API BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE7, LPRECT, DWORD);
    HRESULT DD_API DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE7);
    HRESULT DD_API EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK7);
    HRESULT DD_API EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK7);
    HRESULT DD_API Flip(LPDIRECTDRAWSURFACE7, DWORD);
    HRESULT DD_API GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE7 *);
    HRESULT DD_API UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE7, LPRECT, DWORD, LPDDOVERLAYFX);
    HRESULT DD_API UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE7);
    HRESULT DD_API SetPriority(DWORD);
    HRESULT DD_API GetPriority(LPDWORD);
    HRESULT DD_API SetLOD(DWORD);
    HRESULT DD_API GetLOD(LPDWORD);
};

#endif // MYDIRECTDRAWSURFACESW_HPP

#ifndef MYDIRECTDRAWSURFACE_HPP
#define MYDIRECTDRAWSURFACE_HPP

#include "common.hpp"
#include "MyDirectDrawPaletteProxy.hpp"
#include "MyDirectDrawClipperProxy.hpp"

class MyDirectDrawProxy;

class MyDirectDrawSurfaceProxy :
        public IDirectDrawSurface,
        public IDirectDrawSurface2,
        public IDirectDrawSurface3,
        public IDirectDrawSurface4,
        public IDirectDrawSurface7
{
    int mVersion = 1;
    std::atomic<ULONG> mRefCount;
    logger* mLog               = nullptr;
    com_ptr<IDirectDrawSurface>  mDDS;
    com_ptr<IDirectDrawSurface2> mDDS2;
    com_ptr<IDirectDrawSurface3> mDDS3;
    com_ptr<IDirectDrawSurface4> mDDS4;
    com_ptr<IDirectDrawSurface7> mDDS7;
    com_ptr<MyDirectDrawPaletteProxy> mPalette;
    com_ptr<MyDirectDrawClipperProxy> mClipper;
    MyDirectDrawProxy* mParent = nullptr;

    template<typename SrcT>
    static MyDirectDrawSurfaceProxy* adjPtr(SrcT* src)
    {
        return adjust_pointer<MyDirectDrawSurfaceProxy>(src);
    }

    template<typename SrcT>
    static LPDIRECTDRAWSURFACE adjPtrDDS1(SrcT* src)
    {
        auto ptr = adjust_pointer<MyDirectDrawSurfaceProxy>(src);
        return ptr ? ptr->mDDS.get() : nullptr;
    }

    template<typename SrcT>
    static LPDIRECTDRAWSURFACE2 adjPtrDDS2(SrcT* src)
    {
        auto ptr = adjust_pointer<MyDirectDrawSurfaceProxy>(src);
        return ptr ? ptr->mDDS2.get() : nullptr;
    }

    template<typename SrcT>
    static LPDIRECTDRAWSURFACE3 adjPtrDDS3(SrcT* src)
    {
        auto ptr = adjust_pointer<MyDirectDrawSurfaceProxy>(src);
        return ptr ? ptr->mDDS3.get() : nullptr;
    }

    template<typename SrcT>
    static LPDIRECTDRAWSURFACE4 adjPtrDDS4(SrcT* src)
    {
        auto ptr = adjust_pointer<MyDirectDrawSurfaceProxy>(src);
        return ptr ? ptr->mDDS4.get() : nullptr;
    }

    template<typename SrcT>
    static LPDIRECTDRAWSURFACE7 adjPtrDDS7(SrcT* src)
    {
        auto ptr = adjust_pointer<MyDirectDrawSurfaceProxy>(src);
        return ptr ? ptr->mDDS7.get() : nullptr;
    }

public:
    MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE  dds, MyDirectDrawProxy* parent);
    MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE2 dds, MyDirectDrawProxy* parent);
    MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE3 dds, MyDirectDrawProxy* parent);
    MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE4 dds, MyDirectDrawProxy* parent);
    MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE7 dds, MyDirectDrawProxy* parent);
    virtual ~MyDirectDrawSurfaceProxy();

    LPDIRECTDRAWSURFACE  origSurf1() const { return mDDS.get(); }
    LPDIRECTDRAWSURFACE2 origSurf2() const { return mDDS2.get(); }
    LPDIRECTDRAWSURFACE3 origSurf3() const { return mDDS3.get(); }
    LPDIRECTDRAWSURFACE4 origSurf4() const { return mDDS4.get(); }
    LPDIRECTDRAWSURFACE7 origSurf7() const { return mDDS7.get(); }

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

#endif // MYDIRECTDRAWSURFACE_HPP

#include "MyDirectDrawSurfaceProxy.hpp"

#include "serialize_helpers.hpp"

MyDirectDrawSurfaceProxy::MyDirectDrawSurfaceProxy(logger *log, LPDIRECTDRAWSURFACE dds, MyDirectDrawProxy *parent):
    mRefCount(1),
    mLog(log),
    mDDS(dds),
    mParent(parent)
{
    LOG_FUNCTION();
}

MyDirectDrawSurfaceProxy::MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE2 dds, MyDirectDrawProxy* parent):
    mRefCount(1),
    mLog(log),
    mDDS2(dds),
    mParent(parent)
{
    LOG_FUNCTION();
    mDDS2->QueryInterface(IID_IDirectDrawSurface, (void**)&mDDS);
}

MyDirectDrawSurfaceProxy::MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE3 dds, MyDirectDrawProxy* parent):
    mRefCount(1),
    mLog(log),
    mDDS3(dds),
    mParent(parent)
{
    LOG_FUNCTION();
    mDDS3->QueryInterface(IID_IDirectDrawSurface,  (void**)&mDDS);
    mDDS3->QueryInterface(IID_IDirectDrawSurface2, (void**)&mDDS2);
}

MyDirectDrawSurfaceProxy::MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE4 dds, MyDirectDrawProxy* parent):
    mRefCount(1),
    mLog(log),
    mDDS4(dds),
    mParent(parent)
{
    LOG_FUNCTION();
    mDDS4->QueryInterface(IID_IDirectDrawSurface,  (void**)&mDDS);
    mDDS4->QueryInterface(IID_IDirectDrawSurface2, (void**)&mDDS2);
    mDDS4->QueryInterface(IID_IDirectDrawSurface3, (void**)&mDDS3);
}
MyDirectDrawSurfaceProxy::MyDirectDrawSurfaceProxy(logger* log, LPDIRECTDRAWSURFACE7 dds, MyDirectDrawProxy* parent):
    mRefCount(1),
    mLog(log),
    mDDS7(dds),
    mParent(parent)
{
    LOG_FUNCTION();
    mDDS7->QueryInterface(IID_IDirectDrawSurface,  (void**)&mDDS);
    mDDS7->QueryInterface(IID_IDirectDrawSurface2, (void**)&mDDS2);
    mDDS7->QueryInterface(IID_IDirectDrawSurface3, (void**)&mDDS3);
    mDDS7->QueryInterface(IID_IDirectDrawSurface4, (void**)&mDDS4);
}

MyDirectDrawSurfaceProxy::~MyDirectDrawSurfaceProxy()
{
    LOG_FUNCTION();
}

HRESULT DD_API MyDirectDrawSurfaceProxy::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();

    const std::pair<int,IID> versions[] = {{1,IID_IDirectDrawSurface},
                                           {2,IID_IDirectDrawSurface2},
                                           {3,IID_IDirectDrawSurface3},
                                           {4,IID_IDirectDrawSurface4},
                                           {7,IID_IDirectDrawSurface7}};

    const auto ver = std::find_if(std::begin(versions),
                                  std::end(versions),
                                  [&](const std::pair<int,GUID>& val)
    {
        return riid == val.second;
    });

    if(std::end(versions) == ver)
    {
        mVersion = 1;
        LOG_INFO() << "Unknown DDS version. IID: " << riid;
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }
    else
    {
        mVersion = (*ver).first;
        LOG_INFO() << "DDS version: " << mVersion;
    }

    AddRef();
    if(mVersion >= 1)
    {
        *ppvObj = static_cast<LPDIRECTDRAWSURFACE>(this);
    }
    if(mVersion >= 2)
    {
        if(!mDDS2)
        {
            const HRESULT hr = mDDS->QueryInterface(IID_IDirectDrawSurface2,(void**)&mDDS2);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAWSURFACE2>(this);
    }
    if(mVersion >= 3)
    {
        if(!mDDS3)
        {
            const HRESULT hr = mDDS->QueryInterface(IID_IDirectDrawSurface3,(void**)&mDDS3);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAWSURFACE3>(this);
    }
    if(mVersion >= 4)
    {
        if(!mDDS4)
        {
            const HRESULT hr = mDDS->QueryInterface(IID_IDirectDrawSurface4,(void**)&mDDS4);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAWSURFACE4>(this);
    }
    if(mVersion >= 7)
    {
        if(!mDDS7)
        {
            const HRESULT hr = mDDS->QueryInterface(IID_IDirectDrawSurface7,(void**)&mDDS7);
            if(FAILED(hr))
            {
                return hr;
            }
        }
        *ppvObj = static_cast<LPDIRECTDRAWSURFACE7>(this);
    }
    return S_OK;
}

ULONG   DD_API MyDirectDrawSurfaceProxy::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawSurfaceProxy::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

//IDirectDrawSurface
HRESULT DD_API MyDirectDrawSurfaceProxy::AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSurface)
{
    LOG_FUNCTION();
    return mDDS->AddAttachedSurface(adjPtrDDS1(lpDDSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::AddOverlayDirtyRect(LPRECT lpRect)
{
    LOG_FUNCTION();
    return mDDS->AddOverlayDirtyRect(lpRect);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLT_COLORFILL);
    WRITE_FLAG(dwFlags, DDBLT_DDFX);
    WRITE_FLAG(dwFlags, DDBLT_DDROPS);
    WRITE_FLAG(dwFlags, DDBLT_DEPTHFILL);
    WRITE_FLAG(dwFlags, DDBLT_KEYDESTOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRCOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_ROP);
    WRITE_FLAG(dwFlags, DDBLT_ROTATIONANGLE);
    WRITE_FLAG(dwFlags, DDBLT_KEYDEST);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRC);
    WRITE_FLAG(dwFlags, DDBLT_ASYNC);
    WRITE_FLAG(dwFlags, DDBLT_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDBLT_WAIT);
    write_log(lpDDBltFx, mLog);
    return mDDS->Blt(lpDestRect, adjPtrDDS1(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltBatch(LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS->BltBatch(lpDDBltBatch, dwCount, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLTFAST_DESTCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_SRCCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_WAIT);
    return mDDS->BltFast(dwX, dwY, adjPtrDDS1(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE lpDDSAttachedSurface)
{
    LOG_FUNCTION();
    return mDDS->DeleteAttachedSurface(dwFlags, adjPtrDDS1(lpDDSAttachedSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
    //return mDDS->EnumAttachedSurfaces(lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpfnCallback)
{
    LOG_FUNCTION();
    return mDDS->EnumOverlayZOrders(dwFlags, lpContext, lpfnCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Flip(LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS->Flip(adjPtrDDS1(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE * lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE temp;
    write_log(lpDDSCaps, mLog);
    const HRESULT hr = mDDS->GetAttachedSurface(lpDDSCaps, &temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDAttachedSurface = new MyDirectDrawSurfaceProxy(mLog, temp, mParent);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetBltStatus(DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS->GetBltStatus(dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetCaps(LPDDSCAPS lpDDSCaps)
{
    LOG_FUNCTION();
    const HRESULT hr = mDDS->GetCaps(lpDDSCaps);
    if(SUCCEEDED(hr))
    {
        write_log(lpDDSCaps, mLog);
    }
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetClipper(LPDIRECTDRAWCLIPPER *lplpDDClipper)
{
    LOG_FUNCTION();
    if(mClipper)
    {
        mClipper->AddRef();
        *lplpDDClipper = mClipper.get();
        return S_OK;
    }
    LPDIRECTDRAWCLIPPER temp;
    const HRESULT hr = mDDS->GetClipper(&temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDClipper = new MyDirectDrawClipperProxy(mLog, temp);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
    LOG_FUNCTION();
    return mDDS->GetColorKey(dwFlags, lpDDColorKey);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetDC(HDC *lphDC)
{
    LOG_FUNCTION();
    return mDDS->GetDC(lphDC);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetFlipStatus(DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS->GetFlipStatus(dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetOverlayPosition(LPLONG lplX, LPLONG lplY)
{
    LOG_FUNCTION();
    return mDDS->GetOverlayPosition(lplX, lplY);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetPalette(LPDIRECTDRAWPALETTE *lplpDDPalette)
{
    LOG_FUNCTION();
    if(mPalette)
    {
        mPalette->AddRef();
        *lplpDDPalette = mPalette.get();
        return S_OK;
    }
    LPDIRECTDRAWPALETTE temp;
    const HRESULT hr = mDDS->GetPalette(&temp);
    if(FAILED(hr))
    {
        return hr;
    }
    mPalette = new MyDirectDrawPaletteProxy(mLog, temp);
    mPalette->AddRef();
    *lplpDDPalette = mPalette.get();
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetPixelFormat(LPDDPIXELFORMAT lpDDPixelFormat)
{
    LOG_FUNCTION();
    return mDDS->GetPixelFormat(lpDDPixelFormat);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    const HRESULT hr = mDDS->GetSurfaceDesc(lpDDSurfaceDesc);
    if(SUCCEEDED(hr))
    {
        write_log(lpDDSurfaceDesc, mLog);
    }
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Initialize(LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return mDDS->Initialize(lpDD, lpDDSurfaceDesc);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::IsLost()
{
    LOG_FUNCTION();
    return mDDS->IsLost();
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Lock(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDLOCK_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDLOCK_EVENT);
    WRITE_FLAG(dwFlags, DDLOCK_NOOVERWRITE);
    WRITE_FLAG(dwFlags, DDLOCK_NOSYSLOCK);
    WRITE_FLAG(dwFlags, DDLOCK_DISCARDCONTENTS);
    WRITE_FLAG(dwFlags, DDLOCK_READONLY);
    WRITE_FLAG(dwFlags, DDLOCK_WAIT);
    WRITE_FLAG(dwFlags, DDLOCK_WRITEONLY);
    write_log(lpDDSurfaceDesc, mLog);
    return mDDS->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::ReleaseDC(HDC hDC)
{
    LOG_FUNCTION();
    return mDDS->ReleaseDC(hDC);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Restore()
{
    LOG_FUNCTION();
    return mDDS->Restore();
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetClipper(LPDIRECTDRAWCLIPPER lpDDClipper)
{
    LOG_FUNCTION();
    mClipper.reset(adjust_pointer<MyDirectDrawClipperProxy>(lpDDClipper), true);
    return mDDS->SetClipper(mClipper ? mClipper->getSrcClipper() : nullptr);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetColorKey(DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
    LOG_FUNCTION();
    return mDDS->SetColorKey(dwFlags, lpDDColorKey);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetOverlayPosition(LONG lX, LONG lY)
{
    LOG_FUNCTION();
    return mDDS->SetOverlayPosition(lX, lY);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette)
{
    LOG_FUNCTION();
    mPalette.reset(adjust_pointer<MyDirectDrawPaletteProxy>(lpDDPalette), true);
    return mDDS->SetPalette(mPalette ? mPalette->GetSrcPalette() : nullptr);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Unlock(LPVOID lpRect)
{
    LOG_FUNCTION();
    return mDDS->Unlock(lpRect);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION();
    return mDDS->UpdateOverlay(lpSrcRect, adjPtrDDS1(lpDDDestSurface), lpDestRect, dwFlags, lpDDOverlayFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayDisplay(DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS->UpdateOverlayDisplay(dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE lpDDSReference)
{
    LOG_FUNCTION();
    return mDDS->UpdateOverlayZOrder(dwFlags, adjPtrDDS1(lpDDSReference));
}

//IDirectDrawSurface2
HRESULT DD_API MyDirectDrawSurfaceProxy::AddAttachedSurface(LPDIRECTDRAWSURFACE2 lpDDSurface)
{
    LOG_FUNCTION();
    return mDDS2->AddAttachedSurface(adjPtrDDS2(lpDDSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLT_COLORFILL);
    WRITE_FLAG(dwFlags, DDBLT_DDFX);
    WRITE_FLAG(dwFlags, DDBLT_DDROPS);
    WRITE_FLAG(dwFlags, DDBLT_DEPTHFILL);
    WRITE_FLAG(dwFlags, DDBLT_KEYDESTOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRCOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_ROP);
    WRITE_FLAG(dwFlags, DDBLT_ROTATIONANGLE);
    WRITE_FLAG(dwFlags, DDBLT_KEYDEST);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRC);
    WRITE_FLAG(dwFlags, DDBLT_ASYNC);
    WRITE_FLAG(dwFlags, DDBLT_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDBLT_WAIT);
    write_log(lpDDBltFx, mLog);
    return mDDS2->Blt(lpDestRect, adjPtrDDS2(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLTFAST_DESTCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_SRCCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_WAIT);
    return mDDS2->BltFast(dwX, dwY, adjPtrDDS2(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface)
{
    LOG_FUNCTION();
    return mDDS2->DeleteAttachedSurface(dwFlags, adjPtrDDS2(lpDDSAttachedSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Flip(LPDIRECTDRAWSURFACE2 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS2->Flip(adjPtrDDS2(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE2 *lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE2 temp;
    write_log(lpDDSCaps, mLog);
    const HRESULT hr = mDDS2->GetAttachedSurface(lpDDSCaps, &temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDAttachedSurface = new MyDirectDrawSurfaceProxy(mLog, temp, mParent);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE2 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION();
    return mDDS2->UpdateOverlay(lpSrcRect, adjPtrDDS2(lpDDDestSurface), lpDestRect, dwFlags, lpDDOverlayFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSReference)
{
    LOG_FUNCTION();
    return mDDS2->UpdateOverlayZOrder(dwFlags, adjPtrDDS2(lpDDSReference));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetDDInterface(LPVOID * lplpDD)
{
    LOG_FUNCTION();
    *lplpDD = mParent;
    return S_OK;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::PageLock(DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS2->PageLock(dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::PageUnlock(DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS2->PageUnlock(dwFlags);
}

//IDirectDrawSurface3
HRESULT DD_API MyDirectDrawSurfaceProxy::AddAttachedSurface(LPDIRECTDRAWSURFACE3 lpDDSurface)
{
    LOG_FUNCTION();
    return mDDS3->AddAttachedSurface(adjPtrDDS3(lpDDSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE3 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLT_COLORFILL);
    WRITE_FLAG(dwFlags, DDBLT_DDFX);
    WRITE_FLAG(dwFlags, DDBLT_DDROPS);
    WRITE_FLAG(dwFlags, DDBLT_DEPTHFILL);
    WRITE_FLAG(dwFlags, DDBLT_KEYDESTOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRCOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_ROP);
    WRITE_FLAG(dwFlags, DDBLT_ROTATIONANGLE);
    WRITE_FLAG(dwFlags, DDBLT_KEYDEST);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRC);
    WRITE_FLAG(dwFlags, DDBLT_ASYNC);
    WRITE_FLAG(dwFlags, DDBLT_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDBLT_WAIT);
    write_log(lpDDBltFx, mLog);
    return mDDS3->Blt(lpDestRect, adjPtrDDS3(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE3 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLTFAST_DESTCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_SRCCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_WAIT);
    return mDDS3->BltFast(dwX, dwY, adjPtrDDS3(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE3 lpDDSAttachedSurface)
{
    LOG_FUNCTION();
    return mDDS3->DeleteAttachedSurface(dwFlags, adjPtrDDS3(lpDDSAttachedSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Flip(LPDIRECTDRAWSURFACE3 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS3->Flip(adjPtrDDS3(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetAttachedSurface(LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE3 * lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE3 temp;
    write_log(lpDDSCaps, mLog);
    const HRESULT hr = mDDS3->GetAttachedSurface(lpDDSCaps, &temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDAttachedSurface = new MyDirectDrawSurfaceProxy(mLog, temp, mParent);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE3 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION();
    return mDDS3->UpdateOverlay(lpSrcRect, adjPtrDDS3(lpDDDestSurface), lpDestRect, dwFlags, lpDDOverlayFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE3 lpDDSReference)
{
    LOG_FUNCTION();
    return mDDS3->UpdateOverlayZOrder(dwFlags, adjPtrDDS3(lpDDSReference));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetSurfaceDesc(LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS3->SetSurfaceDesc(lpDDSurfaceDesc, dwFlags);
}


//IDirectDrawSurface4
HRESULT DD_API MyDirectDrawSurfaceProxy::AddAttachedSurface(LPDIRECTDRAWSURFACE4 lpDDSurface)
{
    LOG_FUNCTION();
    return mDDS4->AddAttachedSurface(adjPtrDDS4(lpDDSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE4 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLT_COLORFILL);
    WRITE_FLAG(dwFlags, DDBLT_DDFX);
    WRITE_FLAG(dwFlags, DDBLT_DDROPS);
    WRITE_FLAG(dwFlags, DDBLT_DEPTHFILL);
    WRITE_FLAG(dwFlags, DDBLT_KEYDESTOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRCOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_ROP);
    WRITE_FLAG(dwFlags, DDBLT_ROTATIONANGLE);
    WRITE_FLAG(dwFlags, DDBLT_KEYDEST);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRC);
    WRITE_FLAG(dwFlags, DDBLT_ASYNC);
    WRITE_FLAG(dwFlags, DDBLT_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDBLT_WAIT);
    write_log(lpDDBltFx, mLog);
    return mDDS4->Blt(lpDestRect, adjPtrDDS4(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE4 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLTFAST_DESTCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_SRCCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_WAIT);
    return mDDS4->BltFast(dwX, dwY, adjPtrDDS4(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE4 lpDDSAttachedSurface)
{
    LOG_FUNCTION();
    return mDDS4->DeleteAttachedSurface(dwFlags, adjPtrDDS4(lpDDSAttachedSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
    //return mDDS4->EnumAttachedSurfaces(lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK2 lpfnCallback)
{
    LOG_FUNCTION();
    return mDDS4->EnumOverlayZOrders(dwFlags, lpContext, lpfnCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Flip(LPDIRECTDRAWSURFACE4 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS4->Flip(adjPtrDDS4(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::FreePrivateData(const GUID & g)
{
    LOG_FUNCTION();
    return mDDS4->FreePrivateData(g);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE4 *lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE4 temp;
    const HRESULT hr = mDDS4->GetAttachedSurface(lpDDSCaps, &temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDAttachedSurface = new MyDirectDrawSurfaceProxy(mLog, temp, mParent);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetCaps(LPDDSCAPS2 lpDDSCaps)
{
    LOG_FUNCTION();
    return mDDS4->GetCaps(lpDDSCaps);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetSurfaceDesc(LPDDSURFACEDESC2 lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return mDDS4->GetSurfaceDesc(lpDDSurfaceDesc);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Initialize(LPDIRECTDRAW lpDD, LPDDSURFACEDESC2 lpDDSurfaceDesc)
{
    LOG_FUNCTION();
    return mDDS4->Initialize(lpDD, lpDDSurfaceDesc);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Lock(LPRECT lpDestRect, LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDLOCK_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDLOCK_EVENT);
    WRITE_FLAG(dwFlags, DDLOCK_NOOVERWRITE);
    WRITE_FLAG(dwFlags, DDLOCK_NOSYSLOCK);
    WRITE_FLAG(dwFlags, DDLOCK_DISCARDCONTENTS);
    WRITE_FLAG(dwFlags, DDLOCK_READONLY);
    WRITE_FLAG(dwFlags, DDLOCK_WAIT);
    WRITE_FLAG(dwFlags, DDLOCK_WRITEONLY);
    return mDDS4->Lock(lpDestRect, lpDDSurfaceDesc, dwFlags, hEvent);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Unlock(LPRECT lpRect)
{
    LOG_FUNCTION();
    return mDDS4->Unlock(lpRect);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE4 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION();
    return mDDS4->UpdateOverlay(lpSrcRect, adjPtrDDS4(lpDDDestSurface), lpDestRect, dwFlags, lpDDOverlayFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE4 lpDDSReference)
{
    LOG_FUNCTION();
    return mDDS4->UpdateOverlayZOrder(dwFlags, adjPtrDDS4(lpDDSReference));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetSurfaceDesc(LPDDSURFACEDESC2 lpDDSurfaceDesc, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS4->SetSurfaceDesc(lpDDSurfaceDesc, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetPrivateData(const GUID & guidTag, LPVOID lpData, DWORD cbSize, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS4->SetPrivateData(guidTag, lpData, cbSize, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetPrivateData(const GUID &guidTag, LPVOID lpBuffer, LPDWORD lpcbBufferSize)
{
    LOG_FUNCTION();
    return mDDS4->GetPrivateData(guidTag, lpBuffer, lpcbBufferSize);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetUniquenessValue(LPDWORD lpVal)
{
    LOG_FUNCTION();
    return mDDS4->GetUniquenessValue(lpVal);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::ChangeUniquenessValue()
{
    LOG_FUNCTION();
    return mDDS4->ChangeUniquenessValue();
}

//IDirectDrawSurface7
HRESULT DD_API MyDirectDrawSurfaceProxy::AddAttachedSurface(LPDIRECTDRAWSURFACE7 lpDDSurface)
{
    LOG_FUNCTION();
    return mDDS7->AddAttachedSurface(adjPtrDDS7(lpDDSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Blt(LPRECT lpDestRect, LPDIRECTDRAWSURFACE7 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLT_COLORFILL);
    WRITE_FLAG(dwFlags, DDBLT_DDFX);
    WRITE_FLAG(dwFlags, DDBLT_DDROPS);
    WRITE_FLAG(dwFlags, DDBLT_DEPTHFILL);
    WRITE_FLAG(dwFlags, DDBLT_KEYDESTOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRCOVERRIDE);
    WRITE_FLAG(dwFlags, DDBLT_ROP);
    WRITE_FLAG(dwFlags, DDBLT_ROTATIONANGLE);
    WRITE_FLAG(dwFlags, DDBLT_KEYDEST);
    WRITE_FLAG(dwFlags, DDBLT_KEYSRC);
    WRITE_FLAG(dwFlags, DDBLT_ASYNC);
    WRITE_FLAG(dwFlags, DDBLT_DONOTWAIT);
    WRITE_FLAG(dwFlags, DDBLT_WAIT);
    write_log(lpDDBltFx, mLog);
    return mDDS7->Blt(lpDestRect, adjPtrDDS7(lpDDSrcSurface), lpSrcRect, dwFlags, lpDDBltFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE7 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags)
{
    LOG_FUNCTION();
    WRITE_VAR(dwFlags);
    WRITE_FLAG(dwFlags, DDBLTFAST_DESTCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_SRCCOLORKEY);
    WRITE_FLAG(dwFlags, DDBLTFAST_WAIT);
    return mDDS7->BltFast(dwX, dwY, adjPtrDDS7(lpDDSrcSurface), lpSrcRect, dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::DeleteAttachedSurface(DWORD dwFlags, LPDIRECTDRAWSURFACE7 lpDDSAttachedSurface)
{
    LOG_FUNCTION();
    return mDDS7->DeleteAttachedSurface(dwFlags, adjPtrDDS7(lpDDSAttachedSurface));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumAttachedSurfaces(LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpEnumSurfacesCallback)
{
    LOG_FUNCTION_E();
    //return mDDS7->EnumAttachedSurfaces(lpContext, lpEnumSurfacesCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::EnumOverlayZOrders(DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK7 lpfnCallback)
{
    LOG_FUNCTION();
    return mDDS7->EnumOverlayZOrders(dwFlags, lpContext, lpfnCallback);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::Flip(LPDIRECTDRAWSURFACE7 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
    LOG_FUNCTION();
    return mDDS7->Flip(adjPtrDDS7(lpDDSurfaceTargetOverride), dwFlags);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetAttachedSurface(LPDDSCAPS2 lpDDSCaps, LPDIRECTDRAWSURFACE7 *lplpDDAttachedSurface)
{
    LOG_FUNCTION();
    LPDIRECTDRAWSURFACE7 temp;
    const HRESULT hr = mDDS7->GetAttachedSurface(lpDDSCaps, &temp);
    if(FAILED(hr))
    {
        return hr;
    }
    *lplpDDAttachedSurface = new MyDirectDrawSurfaceProxy(mLog, temp, mParent);
    return hr;
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlay(LPRECT lpSrcRect, LPDIRECTDRAWSURFACE7 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
    LOG_FUNCTION();
    return mDDS7->UpdateOverlay(lpSrcRect, adjPtrDDS7(lpDDDestSurface), lpDestRect, dwFlags, lpDDOverlayFx);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::UpdateOverlayZOrder(DWORD dwFlags, LPDIRECTDRAWSURFACE7 lpDDSReference)
{
    LOG_FUNCTION();
    return mDDS7->UpdateOverlayZOrder(dwFlags, adjPtrDDS7(lpDDSReference));
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetPriority(DWORD dwPriority)
{
    LOG_FUNCTION();
    return mDDS7->SetPriority(dwPriority);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetPriority(LPDWORD lpdwPriority)
{
    LOG_FUNCTION();
    return mDDS7->GetPriority(lpdwPriority);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::SetLOD(DWORD dwMaxLOD)
{
    LOG_FUNCTION();
    return mDDS7->SetLOD(dwMaxLOD);
}
HRESULT DD_API MyDirectDrawSurfaceProxy::GetLOD(LPDWORD lpdwMaxLOD)
{
    LOG_FUNCTION();
    return mDDS7->GetLOD(lpdwMaxLOD);
}

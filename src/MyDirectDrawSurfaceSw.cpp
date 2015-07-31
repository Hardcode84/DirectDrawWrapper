#include "MyDirectDrawSurfaceSw.hpp"

#include "MyDirectDrawSw.hpp"
#include "tools/help_funcs.hpp"
#include "serialize_helpers.hpp"

MyDirectDrawSurfaceSw::MyDirectDrawSurfaceSw(logger* log,
                                             MyDirectDrawSw* parent,
                                             int width,
                                             int height,
                                             int bpp,
                                             bool plain):
    mRefCount(1),
    mLog(log),
    mParent(parent),
    mWidth(width),
    mHeight(height),
    mBpp(bpp),
    mBuffer(new unsigned char[mWidth * mHeight * (mBpp / 8) + 2 * BufferPadding]),
    mIsOffScrPlain(plain)
{
    LOG_FUNCTION();
    mParent->registerSurfInternal(this);
}

MyDirectDrawSurfaceSw::~MyDirectDrawSurfaceSw()
{
    LOG_FUNCTION();
    ReleaseBitmapInternal();
    if(mIsFrontBuffer)
    {
        mParent->setPrimarySurf(nullptr);
    }
    mParent->unregisterSurfInternal(this);
}

HRESULT DD_API MyDirectDrawSurfaceSw::QueryInterface(const IID &riid, LPVOID *ppvObj)
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
    switch(mVersion)
    {
        case 1:
            *ppvObj = static_cast<LPDIRECTDRAWSURFACE>(this);
            break;
        case 2:
            *ppvObj = static_cast<LPDIRECTDRAWSURFACE2>(this);
            break;
        case 3:
            *ppvObj = static_cast<LPDIRECTDRAWSURFACE3>(this);
            break;
        case 4:
            *ppvObj = static_cast<LPDIRECTDRAWSURFACE4>(this);
            break;
        case 7:
            *ppvObj = static_cast<LPDIRECTDRAWSURFACE7>(this);
            break;
        default:
            Release();
            *ppvObj = nullptr;
            LOG_ERROR() << "Unknown DDS version: " << mVersion;
            return E_NOINTERFACE;
    }
    return S_OK;
}

ULONG   DD_API MyDirectDrawSurfaceSw::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawSurfaceSw::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

MyDirectDrawSurfaceSw* MyDirectDrawSurfaceSw::create(LPDDSURFACEDESC desc,
                                                     logger *log,
                                                     MyDirectDrawSw *parent)
{
    logger* mLog = log;
    MyDirectDrawSurfaceSw* ret = nullptr;
    if((desc->dwFlags & DDSD_CAPS) &&
       (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE))
    {
        //primary buffer
        const auto width = parent->getDisplayMode().width;
        const auto height = parent->getDisplayMode().height;
        const auto bpp = parent->getDisplayMode().bpp;
        unsigned bbcount;
        if(desc->dwFlags & DDSD_BACKBUFFERCOUNT)
        {
            bbcount = desc->dwBackBufferCount;
        }
        else
        {
            bbcount = 0;
        }
        ret = new MyDirectDrawSurfaceSw(log,
                                        parent,
                                        width,
                                        height,
                                        bpp,
                                        false);
        ret->mIsFrontBuffer = true;
        for(unsigned i = 0; i < bbcount; ++i)
        {
            auto bb = new MyDirectDrawSurfaceSw(log,
                                                parent,
                                                width,
                                                height,
                                                bpp,
                                                false);
            bb->mIsBackBuffer = true;
            ret->mChain.push_back(bb);
        }
    }
    else if((desc->dwFlags & DDSD_HEIGHT) &&
            (desc->dwFlags & DDSD_WIDTH))
    {
        const auto width = desc->dwWidth;
        const auto height = desc->dwHeight;
        const auto bpp = parent->getDisplayMode().bpp;
        ret = new MyDirectDrawSurfaceSw(log,
                                        parent,
                                        width,
                                        height,
                                        bpp,
                                        (desc->dwFlags & DDSD_CAPS) &&
                                        (desc->ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN));
    }
    else
    {
        LOG_ERROR() << "Unable to create surface";
        write_log(desc, log);
    }

    return ret;
}

HRESULT MyDirectDrawSurfaceSw::BltInternal(LPRECT lpDestRect,
                                           MyDirectDrawSurfaceSw* lpDDSrcSurface,
                                           LPRECT lpSrcRect,
                                           DWORD dwFlags,
                                           LPDDBLTFX lpDDBltFx)
{
    RECT rc1;
    if(nullptr == lpDestRect)
    {
        rc1.left = 0;
        rc1.top = 0;
        rc1.right = mWidth;
        rc1.bottom = mHeight;
        lpDestRect = &rc1;
    }

    if(DDBLT_COLORFILL & dwFlags)
    {
        auto x = lpDestRect->left;
        auto y = lpDestRect->top;
        if(mIsFrontBuffer)
        {
            const auto offset = mParent->getWindowPos();
            x -= offset.first;
            y -= offset.second;
        }
        const auto w = lpDestRect->right  - lpDestRect->left;
        const auto h = lpDestRect->bottom - lpDestRect->top;
        if(8 == mBpp)
        {
            help_funcs::fill_rect(GetBufferPtr(),
                                  x, y,
                                  w, h, pitch(), (UCHAR)lpDDBltFx->dwFillColor);
        }
        else if(16 == mBpp)
        {
            help_funcs::fill_rect(GetBufferPtr(),
                                  x, y,
                                  w, h, pitch(), (USHORT)lpDDBltFx->dwFillColor);
        }
        else if(32 == mBpp)
        {
            help_funcs::fill_rect(GetBufferPtr(),
                                  x, y,
                                  w, h, pitch(), (DWORD)lpDDBltFx->dwFillColor);
        }
        else
        {
            LOG_ERROR() << "Unsupported bpp: " << mBpp;
            return E_FAIL;
        }
        OnChangedInternal();
        return S_OK;
    }
    RECT rc2;
    if(nullptr == lpSrcRect)
    {
        rc2.left = 0;
        rc2.top = 0;
        rc2.right  = lpDDSrcSurface->mWidth;
        rc2.bottom = lpDDSrcSurface->mHeight;
        lpSrcRect = &rc2;
    }
    const auto dstWidth  = lpDestRect->right  - lpDestRect->left;
    const auto dstHeight = lpDestRect->bottom - lpDestRect->top;
    const auto srcWidth  = lpSrcRect->right   - lpSrcRect->left;
    const auto srcHeight = lpSrcRect->bottom  - lpSrcRect->top;
    if((dstWidth == srcWidth) && (dstHeight == srcHeight) &&
            !(dwFlags & DDBLT_DDFX) &&
            !(dwFlags & DDBLT_DDROPS) &&
            !(dwFlags & DDBLT_ROP) &&
            !(dwFlags & DDBLT_ROTATIONANGLE))
    {
       DWORD flags = 0;
       if(dwFlags & DDBLT_KEYDEST) flags |= DDBLTFAST_DESTCOLORKEY;
       if(dwFlags & DDBLT_KEYSRC)  flags |= DDBLTFAST_SRCCOLORKEY;
       return BltFastInternal(lpDestRect->left,lpDestRect->top, lpDDSrcSurface, lpSrcRect, flags);
    }
    LOG_ERROR() << "BltInternal not implemented";
    WRITE_VAR(*lpDestRect);
    WRITE_VAR(*lpSrcRect);
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
    return E_FAIL;
}

HRESULT MyDirectDrawSurfaceSw::BltFastInternal(DWORD dwX,
                                               DWORD dwY,
                                               MyDirectDrawSurfaceSw* lpDDSrcSurface,
                                               LPRECT lpSrcRect,
                                               DWORD dwFlags)
{
    if(mBpp != lpDDSrcSurface->mBpp)
    {
        LOG_ERROR() << "BltFast: bpp not match";
        return E_FAIL;
    }
    if(mIsFrontBuffer)
    {
        const auto offset = mParent->getWindowPos();
        dwX -= offset.first;
        dwY -= offset.second;
    }
    const auto bypp = mBpp / 8;
    const auto srcLeft = (lpSrcRect ? lpSrcRect->left : 0);
    const auto srcTop  = (lpSrcRect ? lpSrcRect->top  : 0);
    const auto srcRight  = (lpSrcRect ? lpSrcRect->right  : lpDDSrcSurface->mWidth);
    const auto srcBottom = (lpSrcRect ? lpSrcRect->bottom : lpDDSrcSurface->mHeight);
    const auto srcPitch = lpDDSrcSurface->pitch();
    const auto dstPitch = pitch();
    const auto pixelsLine = std::min((int)(mWidth  - dwX), (int)(srcRight - srcLeft));
    const auto numLines   = std::min((int)(mHeight - dwY), (int)(srcBottom - srcTop));
    const unsigned char* srcData = lpDDSrcSurface->GetBufferPtr();
    unsigned char*       dstData = GetBufferPtr();

    DDCOLORKEY colorKey;
    if(dwFlags & DDBLTFAST_DESTCOLORKEY)
    {
        colorKey = mSrcColorKey;
    }
    else if(dwFlags & DDBLTFAST_SRCCOLORKEY)
    {
        colorKey = lpDDSrcSurface->mSrcColorKey;
    }
    else
    {
        const auto lineSize = pixelsLine * bypp;
        for(int i = 0; i < numLines; ++i)
        {
            memcpy(dstData + (dwY + i)*dstPitch + dwX*bypp,
                   srcData + (srcTop + i)*srcPitch + srcLeft*bypp,
                   lineSize);
        }
        OnChangedInternal();
        return S_OK;
    }
    if(colorKey.dwColorSpaceHighValue != colorKey.dwColorSpaceLowValue)
    {
        LOG_ERROR() << "Color key space not implemented";
        return E_NOTIMPL;
    }
    else
    {
        const int bpp = mBpp;
        const DWORD ck = colorKey.dwColorSpaceHighValue;
        for(int i = 0; i < numLines; ++i)
        {
            for(int j = 0; j < pixelsLine; ++j)
            {
                if(8 == bpp)
                {
                    const unsigned char index = *(srcData + (srcTop + i)*srcPitch + (srcLeft + j)*bypp);
                    if(index != ck)
                    {
                        *(dstData + (dwY + i)*dstPitch + (dwX + j)*bypp) = index;
                    }
                }
                else if(16 == bpp)
                {
                    const USHORT color = *((USHORT*)(srcData + (srcTop + i)*srcPitch + (srcLeft + j)*bypp));
                    if(color != ck)
                    {
                        *((USHORT*)(dstData + (dwY + i)*dstPitch + (dwX + j)*bypp)) = color;
                    }
                }
                else if(32 == bpp)
                {
                    const DWORD color = *((DWORD*)(srcData + (srcTop + i)*srcPitch + (srcLeft + j)*bypp));
                    if(color != ck)
                    {
                        *((DWORD*)(dstData + (dwY + i)*dstPitch + (dwX + j)*bypp)) = color;
                    }
                }
                else
                {
                    LOG_ERROR() << "Unsupported bpp: " << bpp;
                    return E_FAIL;
                }
            }
        }
    }

    OnChangedInternal();
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::FlipInternal(MyDirectDrawSurfaceSw* targetOverride, DWORD dwFlags)
{
    (void)dwFlags;
    if(!mIsFrontBuffer)
    {
        return DDERR_UNSUPPORTED;
    }

    if(nullptr != targetOverride)
    {
        const auto it = std::find(mChain.begin(),mChain.end(), targetOverride);
        if(mChain.end() == it)
        {
            return DDERR_INVALIDPARAMS;
        }
        std::swap((*it)->mBuffer, mBuffer);
    }
    else if(mChain.empty())
    {
        //nothing
    }
    else if(1 == mChain.size())
    {
        std::swap(mChain[0]->mBuffer, mBuffer);
    }
    else
    {
        std::swap(mChain[0]->mBuffer, mBuffer);
        com_ptr<MyDirectDrawSurfaceSw> front = mChain.front();
        mChain.pop_front();
        mChain.push_back(front);
    }

    const bool result = mParent->presentInternal(0, 0, mWidth, mHeight, 0, 0, this);
    return result ? S_OK : E_FAIL;
}

void MyDirectDrawSurfaceSw::GetPixelFormatInternal(LPDDPIXELFORMAT pf) const
{
    if(pf->dwSize < sizeof(DDPIXELFORMAT))
    {
        LOG_ERROR() << "Invalid DDPIXELFORMAT size:" << pf->dwSize << " req: " << sizeof(DDPIXELFORMAT);
        return;
    }
    pf->dwFlags = DDPF_RGB;
    pf->dwFourCC = 0;
    getColorMask(pf->dwRBitMask, pf->dwGBitMask, pf->dwBBitMask);
    if(8 == mBpp)
    {
        pf->dwFlags |= DDPF_PALETTEINDEXED8;
        pf->dwRGBBitCount = 8;
        pf->dwRGBAlphaBitMask = 0;
    }
    else if(16 == mBpp)
    {
        pf->dwRGBBitCount = 16;
        pf->dwRGBAlphaBitMask = 0;
    }
    else if(32 == mBpp)
    {
        pf->dwRGBBitCount = 32;
        pf->dwRGBAlphaBitMask = 0;
    }
    else
    {
        LOG_ERROR() << "Invalid bpp: " << mBpp;
    }
    //write_log(pf, mLog);
}

void MyDirectDrawSurfaceSw::GetCapsInternal(DDSCAPS *caps) const
{
    caps->dwCaps = 0;
    if(mIsBackBuffer)
    {
        caps->dwCaps |= DDSCAPS_BACKBUFFER;
        caps->dwCaps |= DDSCAPS_COMPLEX;
        //caps->dwCaps |= DDSCAPS_FLIP;
        caps->dwCaps |= DDSCAPS_VIDEOMEMORY;
    }
    else if(mIsFrontBuffer)
    {
        if(!mChain.empty())
        {
            caps->dwCaps |= DDSCAPS_COMPLEX;
        }
        caps->dwCaps |= DDSCAPS_FLIP;
        caps->dwCaps |= DDSCAPS_FRONTBUFFER;
        caps->dwCaps |= DDSCAPS_PRIMARYSURFACE;
        caps->dwCaps |= DDSCAPS_VISIBLE;
        caps->dwCaps |= DDSCAPS_VIDEOMEMORY;
    }
    else
    {
        caps->dwCaps |= DDSCAPS_SYSTEMMEMORY;
        if(mIsOffScrPlain)
        {
            caps->dwCaps |= DDSCAPS_OFFSCREENPLAIN;
        }
    }
}

void MyDirectDrawSurfaceSw::GetDescInternal(LPDDSURFACEDESC desc) const
{
    if(desc->dwSize < sizeof(DDSURFACEDESC))
    {
        LOG_ERROR() << "Invalid DDSURFACEDESC size:" << desc->dwSize << " req: " << sizeof(DDSURFACEDESC);
        return;
    }
    desc->dwFlags = 0;
    desc->dwFlags |= DDSD_CAPS;
    desc->dwFlags |= DDSD_WIDTH;
    desc->dwFlags |= DDSD_HEIGHT;
    desc->dwFlags |= DDSD_PITCH;
    desc->dwFlags |= DDSD_PIXELFORMAT;
    //desc->dwFlags |= DDSD_CKDESTBLT;
    desc->dwFlags |= DDSD_CKSRCBLT;

    GetCapsInternal(&desc->ddsCaps);
    desc->dwWidth  = mWidth;
    desc->dwHeight = mHeight;
    desc->lPitch   = pitch();
    desc->ddpfPixelFormat.dwSize = sizeof(desc->ddpfPixelFormat);
    GetPixelFormatInternal(&desc->ddpfPixelFormat);
    //desc->ddckCKDestBlt = mDstColorKey;
    desc->ddckCKSrcBlt  = mSrcColorKey;

    if(mIsFrontBuffer)
    {
        desc->dwFlags |= DDSD_BACKBUFFERCOUNT;
        desc->dwBackBufferCount = mChain.size();
    }
}

MyDirectDrawSurfaceSw* MyDirectDrawSurfaceSw::GetAttachedSurfaceInternal(const LPDDSCAPS caps)
{
    if((caps->dwCaps & DDSCAPS_BACKBUFFER) &&
       (1 == mChain.size()))
    {
        mChain.front()->AddRef();
        return mChain.front().get();
    }
    return nullptr;
}

void MyDirectDrawSurfaceSw::getColorMask(DWORD& red, DWORD& green, DWORD& blue) const
{
    if(8 == mBpp)
    {
        red   = 0;
        green = 0;
        blue  = 0;
    }
    else if(16 == mBpp)
    {
        red   = 0x0000f800;
        green = 0x000007e0;
        blue  = 0x0000001f;
    }
    else if(32 == mBpp)
    {
        red   = 0x000000ff;
        green = 0x0000ff00;
        blue  = 0x00ff0000;
    }
}

void MyDirectDrawSurfaceSw::OnChangedInternal()
{
    if(mIsFrontBuffer)
    {
        FlipInternal(nullptr, 0);
    }
}

MyDirectDrawPaletteSw* MyDirectDrawSurfaceSw::getPaletteInternal() const
{
    if(mIsFrontBuffer)
    {
        return mPalette.get();
    }
    MyDirectDrawPaletteSw* palette = mPalette.get();
    if(nullptr == palette)
    {
        const MyDirectDrawSurfaceSw* primary = mParent->getPrimarySurf();
        if(nullptr != primary)
        {
            palette = primary->getPaletteInternal();
        }
    }
    return palette;
}
HRESULT MyDirectDrawSurfaceSw::LockInternal(LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags)
{
    if(mIsFrontBuffer && !mParent->isFullscreen())
    {
        LOG_ERROR() << "DONT DO THIS!";
        return E_FAIL;
    }
    GetDescInternal(lpDDSurfaceDesc);
    lpDDSurfaceDesc->dwFlags |= DDSD_LPSURFACE;

    if(nullptr == lpDestRect)
    {
        lpDDSurfaceDesc->lpSurface = GetBufferPtr();
    }
    else
    {
        lpDDSurfaceDesc->lpSurface = GetBufferPtr() +
                lpDestRect->top * lpDDSurfaceDesc->lPitch +
                lpDestRect->left * (mBpp / 8);
    }

    ++mLockCount;
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::UnlockInternal()
{
    if(!isLocked())
    {
        return DDERR_NOTLOCKED;
    }
    if(0 == --mLockCount)
    {
        OnChangedInternal();
    }
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::GetDCInternal(HDC* hdc)
{
    CHECK_PARAM(nullptr != hdc);
    if(0 == mDCCount)
    {
        const HRESULT hr = CreateBitmapInternal();
        if(FAILED(hr))
        {
            *hdc = nullptr;
            return hr;
        }
    }
    ++mDCCount;
    *hdc = mHDC;
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::ReleaseDCInternal(HDC hdc)
{
    CHECK_PARAM(mHDC == hdc);
    CHECK_PARAM(isDCOut());
    if(0 == --mDCCount)
    {
        return FlushBitmapInternal();
    }
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::CreateBitmapInternal()
{
    LOG_FUNCTION();
    if(nullptr == mHDC)
    {
        mHDC = CreateCompatibleDC(mParent->getDC());
        if(nullptr == mHDC)
        {
            LOG_ERROR() << "Unable to create compatible DC: " << getWinError();
            return HRESULT_FROM_WIN32(GetLastError());
        }
    }

    if(!mHBitmap)
    {
        if(mBpp > 8)
        {
            struct
            {
                BITMAPINFO bmi;
                DWORD dummy[3];
            } s;
            s.bmi.bmiHeader.biSize = sizeof(s.bmi.bmiHeader);
            s.bmi.bmiHeader.biWidth = mWidth;
            s.bmi.bmiHeader.biHeight = -mHeight;
            s.bmi.bmiHeader.biPlanes = 1;
            s.bmi.bmiHeader.biBitCount = mBpp;
            s.bmi.bmiHeader.biCompression = BI_RGB;
            s.bmi.bmiHeader.biSizeImage = 0;
            s.bmi.bmiHeader.biXPelsPerMeter = 0;
            s.bmi.bmiHeader.biYPelsPerMeter = 0;
            s.bmi.bmiHeader.biClrUsed = 0;
            s.bmi.bmiHeader.biClrImportant = 0;
            if(mBpp < 32)
            {
                s.bmi.bmiHeader.biCompression = BI_BITFIELDS;
                auto masks = reinterpret_cast<DWORD*>(s.bmi.bmiColors);
                getColorMask(masks[0], masks[1], masks[2]);
            }
            mHBitmap = ::CreateDIBSection(mHDC,
                                          &s.bmi,
                                          DIB_RGB_COLORS,
                                          &mBitmapData,
                                          nullptr,
                                          0);
        }
        else
        {
            struct
            {
                BITMAPINFO bmi;
                RGBQUAD colors[255];
            } s;

            const MyDirectDrawPaletteSw* palette = getPaletteInternal();
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
            s.bmi.bmiHeader.biSize = sizeof(s.bmi.bmiHeader);
            s.bmi.bmiHeader.biWidth = mWidth;
            s.bmi.bmiHeader.biHeight = -mHeight;
            s.bmi.bmiHeader.biPlanes = 1;
            s.bmi.bmiHeader.biBitCount = 8;
            s.bmi.bmiHeader.biCompression = BI_RGB;
            s.bmi.bmiHeader.biSizeImage = 0;
            s.bmi.bmiHeader.biXPelsPerMeter = 0;
            s.bmi.bmiHeader.biYPelsPerMeter = 0;
            s.bmi.bmiHeader.biClrUsed = 0;
            s.bmi.bmiHeader.biClrImportant = 0;
            mHBitmap = ::CreateDIBSection(mHDC,
                                          &s.bmi,
                                          DIB_RGB_COLORS,
                                          &mBitmapData,
                                          nullptr,
                                          0);
        }
        if(!mHBitmap)
        {
            ::DeleteDC(mHDC);
            mHDC = nullptr;
            LOG_ERROR() << "Unable to create bitmap: " << getWinError();
            return HRESULT_FROM_WIN32(GetLastError());
        }
        memcpy(mBitmapData, GetBufferPtr(), bufferSize());
        ::SelectObject(mHDC, mHBitmap.get());
    }
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::FlushBitmapInternal()
{
    LOG_FUNCTION();
    if(nullptr != mBitmapData)
    {
        ::GdiFlush();
        memcpy(GetBufferPtr(), mBitmapData, bufferSize());
        OnChangedInternal();
    }

    if(8 == mBpp)
    {
        //palette can change, so we need to recreate bitmap every time
        mHBitmap.reset();
    }
    return S_OK;
}

HRESULT MyDirectDrawSurfaceSw::ReleaseBitmapInternal()
{
    LOG_FUNCTION();
    FlushBitmapInternal();
    mHBitmap.reset();
    ::DeleteDC(mHDC);
    mHDC = nullptr;
    mBitmapData = nullptr;
    return S_OK;
}


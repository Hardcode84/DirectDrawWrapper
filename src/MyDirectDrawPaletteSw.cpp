#include "MyDirectDrawPaletteSw.hpp"

#include "MyDirectDrawSw.hpp"

MyDirectDrawPaletteSw::MyDirectDrawPaletteSw(logger *log, DWORD caps, const PALETTEENTRY *data, MyDirectDrawSw *parent):
    mRefCount(1),
    mLog(log),
    mParent(parent),
    mCaps(caps)
{
    LOG_FUNCTION();
    int size = 256;
    if(caps & DDPCAPS_8BIT)
    {
        size = 256;
    }
    else if(caps & DDPCAPS_4BIT)
    {
        size = 16;
    }
    else if(caps & DDPCAPS_2BIT)
    {
        size = 4;
    }
    else if(caps & DDPCAPS_1BIT)
    {
        size = 2;
    }
    mEntries.reserve(size);
    for(int i = 0; i < size; ++i)
    {
        mEntries.push_back(data[i]);
    }
}

DWORD MyDirectDrawPaletteSw::asBGR(int index) const
{
    static_assert(sizeof(PaletteEntry) == sizeof(DWORD), "Invalid PALETTEENTRY size");
    union U
    {
        PaletteEntry e;
        DWORD dw;
        U() {}
    } u;
    u.e = mEntries[index];
    return u.dw & 0x00FFFFFF;
}

DWORD MyDirectDrawPaletteSw::asRGB(int index) const
{
    union U
    {
        PALETTEENTRY e;
        DWORD dw;
    } u;
    u.e.peRed   = mEntries[index].peRed;
    u.e.peGreen = mEntries[index].peGreen;
    u.e.peBlue  = mEntries[index].peBlue;
    return u.dw & 0x00FFFFFF;
}

MyDirectDrawPaletteSw::~MyDirectDrawPaletteSw()
{
    LOG_FUNCTION();
}

HRESULT DD_API MyDirectDrawPaletteSw::QueryInterface(const IID &riid, LPVOID *ppvObj)
{
    LOG_FUNCTION();
    (void)riid;
    *ppvObj = nullptr;
    return E_NOINTERFACE;
}

ULONG   DD_API MyDirectDrawPaletteSw::AddRef()
{
    return ++mRefCount;
}

ULONG   DD_API MyDirectDrawPaletteSw::Release()
{
    const ULONG ret = --mRefCount;
    if(0 == ret)
    {
        delete this;
        return 0;
    }
    return ret;
}

HRESULT DD_API MyDirectDrawPaletteSw::GetCaps(LPDWORD lpdwCaps)
{
    LOG_FUNCTION();
    CHECK_PARAM(nullptr != lpdwCaps);
    *lpdwCaps = mCaps;
    return S_OK;
}

HRESULT DD_API MyDirectDrawPaletteSw::GetEntries(DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    CHECK_PARAM(mEntries.size() >= (dwBase + dwNumEntries));
    std::transform(mEntries.begin() + dwBase, mEntries.begin() + (dwBase + dwNumEntries),
                   lpEntries,
                   [](const PaletteEntry& e)
    {
        PALETTEENTRY ret;
        ret.peRed   = e.peRed;
        ret.peGreen = e.peGreen;
        ret.peBlue  = e.peBlue;
        ret.peFlags = e.peFlags;
        return ret;
    });
    return S_OK;
}

HRESULT DD_API MyDirectDrawPaletteSw::Initialize(LPDIRECTDRAW lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable)
{
    LOG_FUNCTION();
    return DDERR_ALREADYINITIALIZED;
}

HRESULT DD_API MyDirectDrawPaletteSw::SetEntries(DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
    LOG_FUNCTION();
    CHECK_PARAM(0 == dwFlags);
    CHECK_PARAM(mEntries.size() >= (dwStartingEntry + dwCount));
    std::copy(lpEntries, lpEntries + dwCount, mEntries.begin() + dwStartingEntry);
    ++mStamp;
    mParent->presentPrimarySurfInternal();
    return S_OK;
}

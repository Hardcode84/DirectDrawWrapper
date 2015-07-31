#ifndef MYDIRECTDRAWPALETTESW_HPP
#define MYDIRECTDRAWPALETTESW_HPP

#include <vector>

#include "common.hpp"

class MyDirectDrawSw;

class MyDirectDrawPaletteSw :
        public IDirectDrawPalette
{
    std::atomic<ULONG> mRefCount;
    logger* mLog = nullptr;
    MyDirectDrawSw* mParent = nullptr;
    struct PaletteEntry
    {
        BYTE peBlue;
        BYTE peGreen;
        BYTE peRed;
        BYTE peFlags;

        PaletteEntry() {}
        PaletteEntry(const PALETTEENTRY& rhs):
            peBlue(rhs.peBlue),
            peGreen(rhs.peGreen),
            peRed(rhs.peRed),
            peFlags(rhs.peFlags) {}
    };

    std::vector<PaletteEntry> mEntries;
    const DWORD mCaps;
    int mStamp = 0;
public:
    MyDirectDrawPaletteSw(logger* log, DWORD caps, const PALETTEENTRY* data, MyDirectDrawSw* parent);
    virtual ~MyDirectDrawPaletteSw();

    int getStamp() const { return mStamp; }
    int count() const { return mEntries.size(); }

    DWORD asBGR(int index) const;
    DWORD asRGB(int index) const;
    const PaletteEntry& entry(int index) const { return mEntries[index]; }
    PALETTEENTRY gdiEntry(int index) const
    {
        const PaletteEntry& e = entry(index);
        PALETTEENTRY ret;
        ret.peBlue  = e.peBlue;
        ret.peGreen = e.peGreen;
        ret.peRed   = e.peRed;
        ret.peFlags = e.peFlags;
        return ret;
    }

    HRESULT DD_API QueryInterface(const IID &riid, LPVOID *ppvObj);
    ULONG   DD_API AddRef();
    ULONG   DD_API Release();

    HRESULT DD_API GetCaps(LPDWORD);
    HRESULT DD_API GetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY);
    HRESULT DD_API Initialize(LPDIRECTDRAW, DWORD, LPPALETTEENTRY);
    HRESULT DD_API SetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY);
};

#endif // MYDIRECTDRAWPALETTESW_HPP

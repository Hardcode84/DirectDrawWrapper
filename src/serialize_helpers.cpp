#include "serialize_helpers.hpp"

void write_log(const LPDDCAPS caps, logger* mLog)
{
    if(nullptr == caps) return;
    WRITE_VAR(caps->dwSize);
    if(sizeof(DDCAPS_DX1) == caps->dwSize)
    {
        const DDCAPS_DX1* caps1 = reinterpret_cast<const DDCAPS_DX1*>(caps);
        WRITE_VAR(caps1->dwCaps);                 // driver specific capabilities
        write_dd_caps(caps1->dwCaps, mLog);
        WRITE_VAR(caps1->dwCaps2);                // more driver specific capabilites
        write_dd_caps2(caps1->dwCaps2, mLog);
        WRITE_VAR(caps1->dwCKeyCaps);             // color key capabilities of the surface
        write_dd_ckey_caps(caps1->dwCKeyCaps,mLog);
        WRITE_VAR(caps1->dwFXCaps);               // driver specific stretching and effects capabilites
        write_dd_fx_caps(caps1->dwFXCaps,mLog);
        WRITE_VAR(caps1->dwFXAlphaCaps);          // alpha driver specific capabilities
        WRITE_VAR(caps1->dwPalCaps);              // palette capabilities
        WRITE_VAR(caps1->dwSVCaps);               // stereo vision capabilities
        WRITE_VAR(caps1->dwAlphaBltConstBitDepths);       // DDBD_2,4,8
        WRITE_VAR(caps1->dwAlphaBltPixelBitDepths);       // DDBD_1,2,4,8
        WRITE_VAR(caps1->dwAlphaBltSurfaceBitDepths);     // DDBD_1,2,4,8
        WRITE_VAR(caps1->dwAlphaOverlayConstBitDepths);   // DDBD_2,4,8
        WRITE_VAR(caps1->dwAlphaOverlayPixelBitDepths);   // DDBD_1,2,4,8
        WRITE_VAR(caps1->dwAlphaOverlaySurfaceBitDepths); // DDBD_1,2,4,8
        WRITE_VAR(caps1->dwZBufferBitDepths);             // DDBD_8,16,24,32
        WRITE_VAR(caps1->dwVidMemTotal);          // total amount of video memory
        WRITE_VAR(caps1->dwVidMemFree);           // amount of free video memory
        WRITE_VAR(caps1->dwMaxVisibleOverlays);   // maximum number of visible overlays
        WRITE_VAR(caps1->dwCurrVisibleOverlays);  // current number of visible overlays
        WRITE_VAR(caps1->dwNumFourCCCodes);       // number of four cc codes
        WRITE_VAR(caps1->dwAlignBoundarySrc);     // source rectangle alignment
        WRITE_VAR(caps1->dwAlignSizeSrc);         // source rectangle byte size
        WRITE_VAR(caps1->dwAlignBoundaryDest);    // dest rectangle alignment
        WRITE_VAR(caps1->dwAlignSizeDest);        // dest rectangle byte size
        WRITE_VAR(caps1->dwAlignStrideAlign);     // stride alignment
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps1->dwRops[i]);   // ROPS supported
        }
        write_log(&caps1->ddsCaps,mLog);                // DDSCAPS structure has all the general capabilities
        WRITE_VAR(caps1->dwMinOverlayStretch);    // minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps1->dwMaxOverlayStretch);    // maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps1->dwMinLiveVideoStretch);  // OBSOLETE! This field remains for compatability reasons only
        WRITE_VAR(caps1->dwMaxLiveVideoStretch);  // OBSOLETE! This field remains for compatability reasons only
        WRITE_VAR(caps1->dwMinHwCodecStretch);    // OBSOLETE! This field remains for compatability reasons only
        WRITE_VAR(caps1->dwMaxHwCodecStretch);    // OBSOLETE! This field remains for compatability reasons only
        WRITE_VAR(caps1->dwReserved1);            // reserved
        WRITE_VAR(caps1->dwReserved2);            // reserved
        WRITE_VAR(caps1->dwReserved3);
    }
    else if(sizeof(DDCAPS_DX3) == caps->dwSize)
    {
        const DDCAPS_DX3* caps3 = reinterpret_cast<const DDCAPS_DX3*>(caps);
        WRITE_VAR(caps3->dwCaps);                 // driver specific capabilities
        write_dd_caps(caps3->dwCaps, mLog);
        WRITE_VAR(caps3->dwCaps2);                // more driver specific capabilites
        write_dd_caps2(caps3->dwCaps2, mLog);
        WRITE_VAR(caps3->dwCKeyCaps);             // color key capabilities of the surface
        write_dd_ckey_caps(caps3->dwCKeyCaps,mLog);
        WRITE_VAR(caps3->dwFXCaps);               // driver specific stretching and effects capabilites
        write_dd_fx_caps(caps3->dwFXCaps,mLog);
        WRITE_VAR(caps3->dwFXAlphaCaps);          // alpha driver specific capabilities
        WRITE_VAR(caps3->dwPalCaps);              // palette capabilities
        WRITE_VAR(caps3->dwSVCaps);               // stereo vision capabilities
        WRITE_VAR(caps3->dwAlphaBltConstBitDepths);       // DDBD_2,4,8
        WRITE_VAR(caps3->dwAlphaBltPixelBitDepths);       // DDBD_1,2,4,8
        WRITE_VAR(caps3->dwAlphaBltSurfaceBitDepths);     // DDBD_1,2,4,8
        WRITE_VAR(caps3->dwAlphaOverlayConstBitDepths);   // DDBD_2,4,8
        WRITE_VAR(caps3->dwAlphaOverlayPixelBitDepths);   // DDBD_1,2,4,8
        WRITE_VAR(caps3->dwAlphaOverlaySurfaceBitDepths); // DDBD_1,2,4,8
        WRITE_VAR(caps3->dwZBufferBitDepths);             // DDBD_8,16,24,32
        WRITE_VAR(caps3->dwVidMemTotal);          // total amount of video memory
        WRITE_VAR(caps3->dwVidMemFree);           // amount of free video memory
        WRITE_VAR(caps3->dwMaxVisibleOverlays);   // maximum number of visible overlays
        WRITE_VAR(caps3->dwCurrVisibleOverlays);  // current number of visible overlays
        WRITE_VAR(caps3->dwNumFourCCCodes);       // number of four cc codes
        WRITE_VAR(caps3->dwAlignBoundarySrc);     // source rectangle alignment
        WRITE_VAR(caps3->dwAlignSizeSrc);         // source rectangle byte size
        WRITE_VAR(caps3->dwAlignBoundaryDest);    // dest rectangle alignment
        WRITE_VAR(caps3->dwAlignSizeDest);        // dest rectangle byte size
        WRITE_VAR(caps3->dwAlignStrideAlign);     // stride alignment
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps3->dwRops[i]);   // ROPS supported
        }
        write_log(&caps3->ddsCaps,mLog);         // DDSCAPS structure has all the general capabilities
        WRITE_VAR(caps3->dwMinOverlayStretch);    // minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwMaxOverlayStretch);    // maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwMinLiveVideoStretch);  // minimum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwMaxLiveVideoStretch);  // maximum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwMinHwCodecStretch);    // minimum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwMaxHwCodecStretch);    // maximum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        WRITE_VAR(caps3->dwReserved1);            // reserved
        WRITE_VAR(caps3->dwReserved2);            // reserved
        WRITE_VAR(caps3->dwReserved3);            // reserved
        WRITE_VAR(caps3->dwSVBCaps);              // driver specific capabilities for System->Vmem blts
        WRITE_VAR(caps3->dwSVBCKeyCaps);          // driver color key capabilities for System->Vmem blts
        WRITE_VAR(caps3->dwSVBFXCaps);            // driver FX capabilities for System->Vmem blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps3->dwSVBRops[i]);// ROPS supported for System->Vmem blts
        }
        WRITE_VAR(caps3->dwVSBCaps);              // driver specific capabilities for Vmem->System blts
        WRITE_VAR(caps3->dwVSBCKeyCaps);          // driver color key capabilities for Vmem->System blts
        WRITE_VAR(caps3->dwVSBFXCaps);            // driver FX capabilities for Vmem->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps3->dwVSBRops[i]);// ROPS supported for Vmem->System blts
        }
        WRITE_VAR(caps3->dwSSBCaps);              // driver specific capabilities for System->System blts
        WRITE_VAR(caps3->dwSSBCKeyCaps);          // driver color key capabilities for System->System blts
        WRITE_VAR(caps3->dwSSBFXCaps);            // driver FX capabilities for System->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps3->dwSSBRops[i]);// ROPS supported for System->System blts
        }
        WRITE_VAR(caps3->dwReserved4);            // reserved
        WRITE_VAR(caps3->dwReserved5);            // reserved
        WRITE_VAR(caps3->dwReserved6);            // reserved
    }
    else if(sizeof(DDCAPS_DX5) == caps->dwSize)
    {
        const DDCAPS_DX5* caps5 = reinterpret_cast<const DDCAPS_DX5*>(caps);
        /*  4*/ WRITE_VAR(caps5->dwCaps);                 // driver specific capabilities
        write_dd_caps(caps5->dwCaps, mLog);
        /*  8*/ WRITE_VAR(caps5->dwCaps2);                // more driver specific capabilites
        write_dd_caps2(caps5->dwCaps2, mLog);
        /*  c*/ WRITE_VAR(caps5->dwCKeyCaps);             // color key capabilities of the surface
        write_dd_ckey_caps(caps5->dwCKeyCaps,mLog);
        /* 10*/ WRITE_VAR(caps5->dwFXCaps);               // driver specific stretching and effects capabilites
        write_dd_fx_caps(caps5->dwFXCaps,mLog);
        /* 14*/ WRITE_VAR(caps5->dwFXAlphaCaps);          // alpha driver specific capabilities
        /* 18*/ WRITE_VAR(caps5->dwPalCaps);              // palette capabilities
        /* 1c*/ WRITE_VAR(caps5->dwSVCaps);               // stereo vision capabilities
        /* 20*/ WRITE_VAR(caps5->dwAlphaBltConstBitDepths);       // DDBD_2,4,8
        /* 24*/ WRITE_VAR(caps5->dwAlphaBltPixelBitDepths);       // DDBD_1,2,4,8
        /* 28*/ WRITE_VAR(caps5->dwAlphaBltSurfaceBitDepths);     // DDBD_1,2,4,8
        /* 2c*/ WRITE_VAR(caps5->dwAlphaOverlayConstBitDepths);   // DDBD_2,4,8
        /* 30*/ WRITE_VAR(caps5->dwAlphaOverlayPixelBitDepths);   // DDBD_1,2,4,8
        /* 34*/ WRITE_VAR(caps5->dwAlphaOverlaySurfaceBitDepths); // DDBD_1,2,4,8
        /* 38*/ WRITE_VAR(caps5->dwZBufferBitDepths);             // DDBD_8,16,24,32
        /* 3c*/ WRITE_VAR(caps5->dwVidMemTotal);          // total amount of video memory
        /* 40*/ WRITE_VAR(caps5->dwVidMemFree);           // amount of free video memory
        /* 44*/ WRITE_VAR(caps5->dwMaxVisibleOverlays);   // maximum number of visible overlays
        /* 48*/ WRITE_VAR(caps5->dwCurrVisibleOverlays);  // current number of visible overlays
        /* 4c*/ WRITE_VAR(caps5->dwNumFourCCCodes);       // number of four cc codes
        /* 50*/ WRITE_VAR(caps5->dwAlignBoundarySrc);     // source rectangle alignment
        /* 54*/ WRITE_VAR(caps5->dwAlignSizeSrc);         // source rectangle byte size
        /* 58*/ WRITE_VAR(caps5->dwAlignBoundaryDest);    // dest rectangle alignment
        /* 5c*/ WRITE_VAR(caps5->dwAlignSizeDest);        // dest rectangle byte size
        /* 60*/ WRITE_VAR(caps5->dwAlignStrideAlign);     // stride alignment
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps5->dwRops[i]);   // ROPS supported
        }
        /* 84*/ write_log(&caps5->ddsCaps, mLog);                // DDSCAPS structure has all the general capabilities
        /* 88*/ WRITE_VAR(caps5->dwMinOverlayStretch);    // minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 8c*/ WRITE_VAR(caps5->dwMaxOverlayStretch);    // maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 90*/ WRITE_VAR(caps5->dwMinLiveVideoStretch);  // minimum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 94*/ WRITE_VAR(caps5->dwMaxLiveVideoStretch);  // maximum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 98*/ WRITE_VAR(caps5->dwMinHwCodecStretch);    // minimum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 9c*/ WRITE_VAR(caps5->dwMaxHwCodecStretch);    // maximum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* a0*/ WRITE_VAR(caps5->dwReserved1);            // reserved
        /* a4*/ WRITE_VAR(caps5->dwReserved2);            // reserved
        /* a8*/ WRITE_VAR(caps5->dwReserved3);            // reserved
        /* ac*/ WRITE_VAR(caps5->dwSVBCaps);              // driver specific capabilities for System->Vmem blts
        /* b0*/ WRITE_VAR(caps5->dwSVBCKeyCaps);          // driver color key capabilities for System->Vmem blts
        /* b4*/ WRITE_VAR(caps5->dwSVBFXCaps);            // driver FX capabilities for System->Vmem blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps5->dwSVBRops[i]);// ROPS supported for System->Vmem blts
        }
        /* d8*/ WRITE_VAR(caps5->dwVSBCaps);              // driver specific capabilities for Vmem->System blts
        /* dc*/ WRITE_VAR(caps5->dwVSBCKeyCaps);          // driver color key capabilities for Vmem->System blts
        /* e0*/ WRITE_VAR(caps5->dwVSBFXCaps);            // driver FX capabilities for Vmem->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps5->dwVSBRops[i]);// ROPS supported for Vmem->System blts
        }
        /*104*/ WRITE_VAR(caps5->dwSSBCaps);              // driver specific capabilities for System->System blts
        /*108*/ WRITE_VAR(caps5->dwSSBCKeyCaps);          // driver color key capabilities for System->System blts
        /*10c*/ WRITE_VAR(caps5->dwSSBFXCaps);            // driver FX capabilities for System->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps5->dwSSBRops[i]);// ROPS supported for System->System blts
        }
        // Members added for DX5:
        /*130*/ WRITE_VAR(caps5->dwMaxVideoPorts);        // maximum number of usable video ports
        /*134*/ WRITE_VAR(caps5->dwCurrVideoPorts);       // current number of video ports used
        /*138*/ WRITE_VAR(caps5->dwSVBCaps2);             // more driver specific capabilities for System->Vmem blts
        /*13c*/ WRITE_VAR(caps5->dwNLVBCaps);               // driver specific capabilities for non-local->local vidmem blts
        /*140*/ WRITE_VAR(caps5->dwNLVBCaps2);              // more driver specific capabilities non-local->local vidmem blts
        /*144*/ WRITE_VAR(caps5->dwNLVBCKeyCaps);           // driver color key capabilities for non-local->local vidmem blts
        /*148*/ WRITE_VAR(caps5->dwNLVBFXCaps);             // driver FX capabilities for non-local->local blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps5->dwNLVBRops[i]); // ROPS supported for non-local->local blts
        }
    }
    else if(sizeof(DDCAPS_DX7) == caps->dwSize)
    {
        const DDCAPS_DX7* caps7 = reinterpret_cast<const DDCAPS_DX7*>(caps);
        /*  4*/ WRITE_VAR(caps7->dwCaps);                 // driver specific capabilities
        write_dd_caps(caps7->dwCaps, mLog);
        /*  8*/ WRITE_VAR(caps7->dwCaps2);                // more driver specific capabilites
        write_dd_caps2(caps7->dwCaps2, mLog);
        /*  c*/ WRITE_VAR(caps7->dwCKeyCaps);             // color key capabilities of the surface
        write_dd_ckey_caps(caps7->dwCKeyCaps,mLog);
        /* 10*/ WRITE_VAR(caps7->dwFXCaps);               // driver specific stretching and effects capabilites
        write_dd_fx_caps(caps7->dwFXCaps,mLog);
        /* 14*/ WRITE_VAR(caps7->dwFXAlphaCaps);          // alpha driver specific capabilities
        /* 18*/ WRITE_VAR(caps7->dwPalCaps);              // palette capabilities
        /* 1c*/ WRITE_VAR(caps7->dwSVCaps);               // stereo vision capabilities
        /* 20*/ WRITE_VAR(caps7->dwAlphaBltConstBitDepths);       // DDBD_2,4,8
        /* 24*/ WRITE_VAR(caps7->dwAlphaBltPixelBitDepths);       // DDBD_1,2,4,8
        /* 28*/ WRITE_VAR(caps7->dwAlphaBltSurfaceBitDepths);     // DDBD_1,2,4,8
        /* 2c*/ WRITE_VAR(caps7->dwAlphaOverlayConstBitDepths);   // DDBD_2,4,8
        /* 30*/ WRITE_VAR(caps7->dwAlphaOverlayPixelBitDepths);   // DDBD_1,2,4,8
        /* 34*/ WRITE_VAR(caps7->dwAlphaOverlaySurfaceBitDepths); // DDBD_1,2,4,8
        /* 38*/ WRITE_VAR(caps7->dwZBufferBitDepths);             // DDBD_8,16,24,32
        /* 3c*/ WRITE_VAR(caps7->dwVidMemTotal);          // total amount of video memory
        /* 40*/ WRITE_VAR(caps7->dwVidMemFree);           // amount of free video memory
        /* 44*/ WRITE_VAR(caps7->dwMaxVisibleOverlays);   // maximum number of visible overlays
        /* 48*/ WRITE_VAR(caps7->dwCurrVisibleOverlays);  // current number of visible overlays
        /* 4c*/ WRITE_VAR(caps7->dwNumFourCCCodes);       // number of four cc codes
        /* 50*/ WRITE_VAR(caps7->dwAlignBoundarySrc);     // source rectangle alignment
        /* 54*/ WRITE_VAR(caps7->dwAlignSizeSrc);         // source rectangle byte size
        /* 58*/ WRITE_VAR(caps7->dwAlignBoundaryDest);    // dest rectangle alignment
        /* 5c*/ WRITE_VAR(caps7->dwAlignSizeDest);        // dest rectangle byte size
        /* 60*/ WRITE_VAR(caps7->dwAlignStrideAlign);     // stride alignment
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps7->dwRops[i]);   // ROPS supported
        }
        /* 84*/ write_log(&caps7->ddsOldCaps, mLog);             // Was DDSCAPS  ddsCaps. ddsCaps is of type DDSCAPS2 for DX6
        /* 88*/ WRITE_VAR(caps7->dwMinOverlayStretch);    // minimum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 8c*/ WRITE_VAR(caps7->dwMaxOverlayStretch);    // maximum overlay stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 90*/ WRITE_VAR(caps7->dwMinLiveVideoStretch);  // minimum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 94*/ WRITE_VAR(caps7->dwMaxLiveVideoStretch);  // maximum live video stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 98*/ WRITE_VAR(caps7->dwMinHwCodecStretch);    // minimum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* 9c*/ WRITE_VAR(caps7->dwMaxHwCodecStretch);    // maximum hardware codec stretch factor multiplied by 1000, eg 1000 == 1.0, 1300 == 1.3
        /* a0*/ WRITE_VAR(caps7->dwReserved1);            // reserved
        /* a4*/ WRITE_VAR(caps7->dwReserved2);            // reserved
        /* a8*/ WRITE_VAR(caps7->dwReserved3);            // reserved
        /* ac*/ WRITE_VAR(caps7->dwSVBCaps);              // driver specific capabilities for System->Vmem blts
        /* b0*/ WRITE_VAR(caps7->dwSVBCKeyCaps);          // driver color key capabilities for System->Vmem blts
        /* b4*/ WRITE_VAR(caps7->dwSVBFXCaps);            // driver FX capabilities for System->Vmem blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps7->dwSVBRops[i]);// ROPS supported for System->Vmem blts
        }
        /* d8*/ WRITE_VAR(caps7->dwVSBCaps);              // driver specific capabilities for Vmem->System blts
        /* dc*/ WRITE_VAR(caps7->dwVSBCKeyCaps);          // driver color key capabilities for Vmem->System blts
        /* e0*/ WRITE_VAR(caps7->dwVSBFXCaps);            // driver FX capabilities for Vmem->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps7->dwVSBRops[i]);// ROPS supported for Vmem->System blts
        }
        /*104*/ WRITE_VAR(caps7->dwSSBCaps);              // driver specific capabilities for System->System blts
        /*108*/ WRITE_VAR(caps7->dwSSBCKeyCaps);          // driver color key capabilities for System->System blts
        /*10c*/ WRITE_VAR(caps7->dwSSBFXCaps);            // driver FX capabilities for System->System blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps7->dwSSBRops[i]);// ROPS supported for System->System blts
        }
        /*130*/ WRITE_VAR(caps7->dwMaxVideoPorts);        // maximum number of usable video ports
        /*134*/ WRITE_VAR(caps7->dwCurrVideoPorts);       // current number of video ports used
        /*138*/ WRITE_VAR(caps7->dwSVBCaps2);             // more driver specific capabilities for System->Vmem blts
        /*13c*/ WRITE_VAR(caps7->dwNLVBCaps);               // driver specific capabilities for non-local->local vidmem blts
        /*140*/ WRITE_VAR(caps7->dwNLVBCaps2);              // more driver specific capabilities non-local->local vidmem blts
        /*144*/ WRITE_VAR(caps7->dwNLVBCKeyCaps);           // driver color key capabilities for non-local->local vidmem blts
        /*148*/ WRITE_VAR(caps7->dwNLVBFXCaps);             // driver FX capabilities for non-local->local blts
        for(int i = 0; i < DD_ROP_SPACE; ++i)
        {
            WRITE_VAR(caps7->dwNLVBRops[i]); // ROPS supported for non-local->local blts
        }
        // Members added for DX6 release
        /*16c*/ WRITE_VAR(caps7->ddsCaps.dwCaps);               // Surface Caps
        WRITE_VAR(caps7->ddsCaps.dwCaps2);
        WRITE_VAR(caps7->ddsCaps.dwCaps3);
        WRITE_VAR(caps7->ddsCaps.dwCaps4);
    }
}

void write_log(const LPDDSURFACEDESC desc, logger* mLog)
{
    if(nullptr == desc) return;
    WRITE_VAR(desc->dwSize);
    if(desc->dwSize < sizeof(DDSURFACEDESC)) return;
    WRITE_VAR(desc->dwFlags);                // determines what fields are valid
    WRITE_FLAG(desc->dwFlags,DDSD_ALPHABITDEPTH);
    WRITE_FLAG(desc->dwFlags,DDSD_BACKBUFFERCOUNT);
    WRITE_FLAG(desc->dwFlags,DDSD_CAPS);
    WRITE_FLAG(desc->dwFlags,DDSD_CKDESTBLT);
    WRITE_FLAG(desc->dwFlags,DDSD_CKDESTOVERLAY);
    WRITE_FLAG(desc->dwFlags,DDSD_CKSRCBLT);
    WRITE_FLAG(desc->dwFlags,DDSD_CKSRCOVERLAY);
    WRITE_FLAG(desc->dwFlags,DDSD_HEIGHT);
    WRITE_FLAG(desc->dwFlags,DDSD_LINEARSIZE);
    WRITE_FLAG(desc->dwFlags,DDSD_LPSURFACE);
    WRITE_FLAG(desc->dwFlags,DDSD_MIPMAPCOUNT);
    WRITE_FLAG(desc->dwFlags,DDSD_PITCH);
    WRITE_FLAG(desc->dwFlags,DDSD_PIXELFORMAT);
    WRITE_FLAG(desc->dwFlags,DDSD_REFRESHRATE);
    WRITE_FLAG(desc->dwFlags,DDSD_TEXTURESTAGE);
    WRITE_FLAG(desc->dwFlags,DDSD_WIDTH);
    WRITE_FLAG(desc->dwFlags,DDSD_ZBUFFERBITDEPTH);
    WRITE_VAR(desc->dwHeight);               // height of surface to be created
    WRITE_VAR(desc->dwWidth);                // width of input surface
    WRITE_VAR(desc->lPitch);                 // distance to start of next line (return value only)
    WRITE_VAR(desc->dwLinearSize);           // Formless late-allocated optimized surface size
    WRITE_VAR(desc->dwBackBufferCount);      // number of back buffers requested
    WRITE_VAR(desc->dwMipMapCount);          // number of mip-map levels requested
    WRITE_VAR(desc->dwZBufferBitDepth);      // depth of Z buffer requested
    WRITE_VAR(desc->dwRefreshRate);          // refresh rate (used when display mode is described)
    WRITE_VAR(desc->dwAlphaBitDepth);        // depth of alpha buffer requested
    WRITE_VAR(desc->dwReserved);             // reserved
    WRITE_VAR(desc->lpSurface);              // pointer to the associated surface memory
    WRITE_VAR(desc->ddckCKDestOverlay);      // color key for destination overlay use
    WRITE_VAR(desc->ddckCKDestBlt);          // color key for destination blt use
    WRITE_VAR(desc->ddckCKSrcOverlay);       // color key for source overlay use
    WRITE_VAR(desc->ddckCKSrcBlt);           // color key for source blt use
    write_log(&desc->ddpfPixelFormat,mLog);        // pixel format description of the surface
    write_log(&desc->ddsCaps,mLog);                // direct draw surface capabilities
}

void write_log(const DDPIXELFORMAT* pf, logger* mLog)
{
    if(nullptr == pf) return;
    WRITE_VAR(pf->dwSize);
    if(pf->dwSize < sizeof(DDPIXELFORMAT)) return;
    WRITE_VAR(pf->dwFlags);                // pixel format flags
    WRITE_FLAG(pf->dwFlags,DDPF_ALPHA);
    WRITE_FLAG(pf->dwFlags,DDPF_ALPHAPIXELS);
    WRITE_FLAG(pf->dwFlags,DDPF_ALPHAPREMULT);
    WRITE_FLAG(pf->dwFlags,DDPF_COMPRESSED);
    WRITE_FLAG(pf->dwFlags,DDPF_FOURCC);
    WRITE_FLAG(pf->dwFlags,DDPF_LUMINANCE);
    WRITE_FLAG(pf->dwFlags,DDPF_PALETTEINDEXED1);
    WRITE_FLAG(pf->dwFlags,DDPF_PALETTEINDEXED2);
    WRITE_FLAG(pf->dwFlags,DDPF_PALETTEINDEXED4);
    WRITE_FLAG(pf->dwFlags,DDPF_PALETTEINDEXED8);
    WRITE_FLAG(pf->dwFlags,DDPF_PALETTEINDEXEDTO8);
    WRITE_FLAG(pf->dwFlags,DDPF_RGB);
    WRITE_FLAG(pf->dwFlags,DDPF_RGBTOYUV);
    WRITE_FLAG(pf->dwFlags,DDPF_STENCILBUFFER);
    WRITE_FLAG(pf->dwFlags,DDPF_YUV);
    WRITE_FLAG(pf->dwFlags,DDPF_ZBUFFER);
    WRITE_FLAG(pf->dwFlags,DDPF_ZPIXELS);
    WRITE_VAR(pf->dwFourCC);               // (FOURCC code)
    WRITE_VAR(pf->dwRGBBitCount);          // how many bits per pixel
    WRITE_VAR(pf->dwYUVBitCount);          // how many bits per pixel
    WRITE_VAR(pf->dwZBufferBitDepth);      // how many total bits/pixel in z buffer (including any stencil bits)
    WRITE_VAR(pf->dwAlphaBitDepth);        // how many bits for alpha channels
    WRITE_VAR(pf->dwLuminanceBitCount);    // how many bits per pixel
    WRITE_VAR(pf->dwBumpBitCount);         // how many bits per "buxel", total
    WRITE_VAR(pf->dwPrivateFormatBitCount);// Bits per pixel of private driver formats. Only valid in texture
                                           // format list and if DDPF_D3DFORMAT is set
    WRITE_VAR(pf->dwRBitMask);             // mask for red bit
    WRITE_VAR(pf->dwYBitMask);             // mask for Y bits
    WRITE_VAR(pf->dwStencilBitDepth);      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
    WRITE_VAR(pf->dwLuminanceBitMask);     // mask for luminance bits
    WRITE_VAR(pf->dwBumpDuBitMask);        // mask for bump map U delta bits
    WRITE_VAR(pf->dwOperations);           // DDPF_D3DFORMAT Operations
    WRITE_VAR(pf->dwGBitMask);             // mask for green bits
    WRITE_VAR(pf->dwUBitMask);             // mask for U bits
    WRITE_VAR(pf->dwZBitMask);             // mask for Z bits
    WRITE_VAR(pf->dwBumpDvBitMask);        // mask for bump map V delta bits
    WRITE_VAR(pf->MultiSampleCaps.wFlipMSTypes);
    WRITE_VAR(pf->MultiSampleCaps.wBltMSTypes);
    WRITE_VAR(pf->dwBBitMask);             // mask for blue bits
    WRITE_VAR(pf->dwVBitMask);             // mask for V bits
    WRITE_VAR(pf->dwStencilBitMask);       // mask for stencil bits
    WRITE_VAR(pf->dwBumpLuminanceBitMask); // mask for luminance in bump map
    WRITE_VAR(pf->dwRGBAlphaBitMask);      // mask for alpha channel
    WRITE_VAR(pf->dwYUVAlphaBitMask);      // mask for alpha channel
    WRITE_VAR(pf->dwLuminanceAlphaBitMask);// mask for alpha channel
    WRITE_VAR(pf->dwRGBZBitMask);          // mask for Z channel
    WRITE_VAR(pf->dwYUVZBitMask);          // mask for Z channel
}

void write_log(const DDSCAPS *caps, logger* mLog)
{
    WRITE_VAR(caps->dwCaps);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_3DDEVICE);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_ALPHA);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_BACKBUFFER);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_COMPLEX);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_FLIP);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_FRONTBUFFER);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_LOCALVIDMEM);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_MIPMAP);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_NONLOCALVIDMEM);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_OFFSCREENPLAIN);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_OWNDC);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_PALETTE);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_PRIMARYSURFACE);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_SYSTEMMEMORY);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_TEXTURE);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_VIDEOMEMORY);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_VISIBLE);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_WRITEONLY);
    WRITE_FLAG(caps->dwCaps,DDSCAPS_ZBUFFER);
}

void write_log(const LPDDBLTFX bltFx, logger* mLog)
{
    if(nullptr == bltFx) return;
    WRITE_VAR(bltFx->dwSize);
    if(bltFx->dwSize < sizeof(DDBLTFX)) return;
    WRITE_VAR(bltFx->dwDDFX);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ARITHSTRETCHY);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_MIRRORLEFTRIGHT);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_MIRRORUPDOWN);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_NOTEARING);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ROTATE180);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ROTATE270);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ROTATE90);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ZBUFFERBASEDEST);
    WRITE_FLAG(bltFx->dwDDFX,DDBLTFX_ZBUFFERRANGE);
    WRITE_VAR(bltFx->dwROP);
    WRITE_VAR(bltFx->dwRotationAngle);                // Rotation angle for blt
    WRITE_VAR(bltFx->dwZBufferOpCode);                // ZBuffer compares
    WRITE_VAR(bltFx->dwZBufferLow);                   // Low limit of Z buffer
    WRITE_VAR(bltFx->dwZBufferHigh);                  // High limit of Z buffer
    WRITE_VAR(bltFx->dwZBufferBaseDest);              // Destination base value
    WRITE_VAR(bltFx->dwZDestConstBitDepth);           // Bit depth used to specify Z constant for destination
    WRITE_VAR(bltFx->dwZDestConst);                   // Constant to use as Z buffer for dest
    WRITE_VAR(bltFx->lpDDSZBufferDest);   // Surface to use as Z buffer for dest
    WRITE_VAR(bltFx->dwZSrcConstBitDepth);            // Bit depth used to specify Z constant for source
    WRITE_VAR(bltFx->dwZSrcConst);                    // Constant to use as Z buffer for src
    WRITE_VAR(bltFx->lpDDSZBufferSrc);    // Surface to use as Z buffer for src
    WRITE_VAR(bltFx->dwAlphaEdgeBlendBitDepth);       // Bit depth used to specify constant for alpha edge blend
    WRITE_VAR(bltFx->dwAlphaEdgeBlend);               // Alpha for edge blending
    WRITE_VAR(bltFx->dwReserved);
    WRITE_VAR(bltFx->dwAlphaDestConstBitDepth);       // Bit depth used to specify alpha constant for destination
    WRITE_VAR(bltFx->dwAlphaDestConst);               // Constant to use as Alpha Channel
    WRITE_VAR(bltFx->lpDDSAlphaDest);     // Surface to use as Alpha Channel
    WRITE_VAR(bltFx->dwAlphaSrcConstBitDepth);        // Bit depth used to specify alpha constant for source
    WRITE_VAR(bltFx->dwAlphaSrcConst);                // Constant to use as Alpha Channel
    WRITE_VAR(bltFx->lpDDSAlphaSrc);      // Surface to use as Alpha Channel
    WRITE_VAR(bltFx->dwFillColor);                    // color in RGB or Palettized
    WRITE_VAR(bltFx->dwFillDepth);                    // depth value for z-buffer
    WRITE_VAR(bltFx->dwFillPixel);                    // pixel value for RGBA or RGBZ
    WRITE_VAR(bltFx->lpDDSPattern);       // Surface to use as pattern
    WRITE_VAR(bltFx->ddckDestColorkey);               // DestColorkey override
    WRITE_VAR(bltFx->ddckSrcColorkey);                // SrcColorkey override
}

void write_dd_caps(DWORD dwCaps, logger* mLog)
{
    WRITE_FLAG(dwCaps,DDCAPS_3D);
    WRITE_FLAG(dwCaps,DDCAPS_ALIGNBOUNDARYDEST);
    WRITE_FLAG(dwCaps,DDCAPS_ALIGNBOUNDARYSRC);
    WRITE_FLAG(dwCaps,DDCAPS_ALIGNSIZEDEST);
    WRITE_FLAG(dwCaps,DDCAPS_ALIGNSIZESRC);
    WRITE_FLAG(dwCaps,DDCAPS_ALIGNSTRIDE);
    WRITE_FLAG(dwCaps,DDCAPS_ALPHA);
    WRITE_FLAG(dwCaps,DDCAPS_BANKSWITCHED);
    WRITE_FLAG(dwCaps,DDCAPS_BLT);
    WRITE_FLAG(dwCaps,DDCAPS_BLTCOLORFILL);
    WRITE_FLAG(dwCaps,DDCAPS_BLTDEPTHFILL);
    WRITE_FLAG(dwCaps,DDCAPS_BLTFOURCC);
    WRITE_FLAG(dwCaps,DDCAPS_BLTQUEUE);
    WRITE_FLAG(dwCaps,DDCAPS_BLTSTRETCH);
    WRITE_FLAG(dwCaps,DDCAPS_CANBLTSYSMEM);
    WRITE_FLAG(dwCaps,DDCAPS_CANCLIP);
    WRITE_FLAG(dwCaps,DDCAPS_CANCLIPSTRETCHED);
    WRITE_FLAG(dwCaps,DDCAPS_COLORKEY);
    WRITE_FLAG(dwCaps,DDCAPS_COLORKEYHWASSIST);
    WRITE_FLAG(dwCaps,DDCAPS_GDI);
    WRITE_FLAG(dwCaps,DDCAPS_NOHARDWARE);
    WRITE_FLAG(dwCaps,DDCAPS_OVERLAY);
    WRITE_FLAG(dwCaps,DDCAPS_OVERLAYCANTCLIP);
    WRITE_FLAG(dwCaps,DDCAPS_OVERLAYFOURCC);
    WRITE_FLAG(dwCaps,DDCAPS_OVERLAYSTRETCH);
    WRITE_FLAG(dwCaps,DDCAPS_PALETTE);
    WRITE_FLAG(dwCaps,DDCAPS_PALETTEVSYNC);
    WRITE_FLAG(dwCaps,DDCAPS_READSCANLINE);
    WRITE_FLAG(dwCaps,DDCAPS_VBI);
    WRITE_FLAG(dwCaps,DDCAPS_ZBLTS);
    WRITE_FLAG(dwCaps,DDCAPS_ZOVERLAYS);
}

void write_dd_caps2(DWORD dwCaps2, logger* mLog)
{
    WRITE_FLAG(dwCaps2,DDCAPS2_AUTOFLIPOVERLAY);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANBOBHARDWARE);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANBOBINTERLEAVED);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANBOBNONINTERLEAVED);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANCALIBRATEGAMMA);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANDROPZ16BIT);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANFLIPODDEVEN);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANMANAGETEXTURE);
    WRITE_FLAG(dwCaps2,DDCAPS2_CANRENDERWINDOWED);
    WRITE_FLAG(dwCaps2,DDCAPS2_CERTIFIED);
    WRITE_FLAG(dwCaps2,DDCAPS2_COLORCONTROLPRIMARY);
    WRITE_FLAG(dwCaps2,DDCAPS2_COLORCONTROLOVERLAY);
    WRITE_FLAG(dwCaps2,DDCAPS2_COPYFOURCC);
    WRITE_FLAG(dwCaps2,DDCAPS2_FLIPINTERVAL);
    WRITE_FLAG(dwCaps2,DDCAPS2_FLIPNOVSYNC);
    WRITE_FLAG(dwCaps2,DDCAPS2_NO2DDURING3DSCENE);
    WRITE_FLAG(dwCaps2,DDCAPS2_NONLOCALVIDMEM);
    WRITE_FLAG(dwCaps2,DDCAPS2_NONLOCALVIDMEMCAPS);
    WRITE_FLAG(dwCaps2,DDCAPS2_NOPAGELOCKREQUIRED);
    WRITE_FLAG(dwCaps2,DDCAPS2_PRIMARYGAMMA);
    WRITE_FLAG(dwCaps2,DDCAPS2_STEREO);
    WRITE_FLAG(dwCaps2,DDCAPS2_TEXMANINNONLOCALVIDMEM);
    WRITE_FLAG(dwCaps2,DDCAPS2_VIDEOPORT);
    WRITE_FLAG(dwCaps2,DDCAPS2_WIDESURFACES);
}

void write_dd_ckey_caps(DWORD dwCKeyCaps, logger* mLog)
{
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTBLT);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTBLTCLRSPACE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTBLTCLRSPACEYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTBLTYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTOVERLAY);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTOVERLAYCLRSPACE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTOVERLAYCLRSPACEYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTOVERLAYONEACTIVE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_DESTOVERLAYYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_NOCOSTOVERLAY);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCBLT);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCBLTCLRSPACE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCBLTCLRSPACEYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCBLTYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCOVERLAY);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCOVERLAYCLRSPACE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCOVERLAYCLRSPACEYUV);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCOVERLAYONEACTIVE);
    WRITE_FLAG(dwCKeyCaps,DDCKEYCAPS_SRCOVERLAYYUV);
}

void write_dd_fx_caps(DWORD dwFXCaps, logger* mLog)
{
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTALPHA);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTARITHSTRETCHY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTARITHSTRETCHYN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTFILTER);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTMIRRORLEFTRIGHT);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTMIRRORUPDOWN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTROTATION);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTROTATION90);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSHRINKX);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSHRINKXN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSHRINKY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSHRINKYN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSTRETCHX);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSTRETCHXN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSTRETCHY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTSTRETCHYN);
    //WRITE_FLAG(dwFXCaps,DDFXCAPS_BLTTRANSFORM);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYALPHA);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYARITHSTRETCHY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYARITHSTRETCHYN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYFILTER);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYMIRRORLEFTRIGHT);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYMIRRORUPDOWN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSHRINKX);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSHRINKXN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSHRINKY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSHRINKYN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSTRETCHX);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSTRETCHXN);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSTRETCHY);
    WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYSTRETCHYN);
    //WRITE_FLAG(dwFXCaps,DDFXCAPS_OVERLAYTRANSFORM);
}

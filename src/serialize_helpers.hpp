#ifndef SERIALIZE_HELPERS_HPP
#define SERIALIZE_HELPERS_HPP

#include "common.hpp"

void write_log(const LPDDCAPS caps, logger* mLog);
void write_log(const LPDDSURFACEDESC desc, logger* mLog);
void write_log(const DDPIXELFORMAT* pf, logger* mLog);
void write_log(const DDSCAPS* caps, logger* mLog);
void write_log(const LPDDBLTFX bltFx, logger* mLog);
void write_dd_caps(DWORD dwCaps, logger* mLog);
void write_dd_caps2(DWORD dwCaps2, logger* mLog);
void write_dd_ckey_caps(DWORD dwCKeyCaps, logger* mLog);
void write_dd_fx_caps(DWORD dwFXCaps, logger* mLog);

#endif // SERIALIZE_HELPERS_HPP

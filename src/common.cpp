#include "common.hpp"

winerr_wrapper getWinError()
{
    return winerr_wrapper(GetLastError());
}

winerr_wrapper getWinErrorFromHr(HRESULT hr)
{
    return winerr_wrapper(HRESULT_CODE(hr));
}


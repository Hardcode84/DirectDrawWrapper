#include "window_hook.hpp"

#include "../common.hpp"

std::map<HWND, window_hook*> window_hook::hooks;

window_hook::window_hook(logger *log):
    mLog(log)
{
}

window_hook::~window_hook()
{
    unhookWindow();
}

LRESULT CALLBACK window_hook::myStaticWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    window_hook* pThis = hooks[hwnd];
    if(nullptr == pThis)
    {
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return pThis->myWndProc(hwnd, uMsg, wParam, lParam);
}

LRESULT window_hook::myWndProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam) const
{
    if(mHandler)
    {
        return mHandler(hwnd, uMsg, wParam, lParam, mOldWndProc);
    }
    return mOldWndProc(hwnd, uMsg, wParam, lParam);
}

bool window_hook::hookWindow(HWND hwnd)
{
    unhookWindow();
    if(hwnd)
    {
        const auto ret = hooks.insert(std::make_pair(hwnd, this));
        if(!ret.second)
        {
            LOG_ERROR() << "Hook already exust";
            return false;
        }
        mOldWndProc = (WNDPROC)::GetWindowLongPtr(hwnd, GWLP_WNDPROC);
        if(nullptr == mOldWndProc)
        {
            hooks.erase(ret.first);
            LOG_ERROR() << "Unable to get wndProc: " << getWinError();
            return false;
        }
        if(0 == ::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)&window_hook::myStaticWndProc))
        {
            hooks.erase(ret.first);
            LOG_ERROR() << "Unable to set wndProc: " << getWinError();
            return false;
        }
    }
    mHWnd = hwnd;
    return true;
}

void window_hook::unhookWindow()
{
    if(nullptr != mHWnd)
    {
        ::SetWindowLongPtr(mHWnd, GWLP_WNDPROC,  (LONG_PTR)mOldWndProc);
        hooks.erase(mHWnd);
        mOldWndProc = nullptr;
        mHWnd = nullptr;
    }
}

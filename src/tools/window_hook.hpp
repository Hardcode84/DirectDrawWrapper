#ifndef WINDOW_HOOK_HPP
#define WINDOW_HOOK_HPP

#include <windows.h>
#include <functional>
#include <map>
#include "../simple_logger/logger.hpp"

class window_hook
{
    std::function<LRESULT(HWND,UINT,WPARAM,LPARAM,WNDPROC)> mHandler;
    HWND mHWnd = nullptr;
    WNDPROC mOldWndProc = nullptr;
    logger* mLog;
    static LRESULT CALLBACK myStaticWndProc(HWND,UINT,WPARAM,LPARAM);
    LRESULT myWndProc(HWND,UINT,WPARAM,LPARAM) const;
    static std::map<HWND, window_hook*> hooks;
public:
    window_hook(logger* log);

    template<typename T>
    window_hook(logger* log, const T& h):
        mHandler(h),
        mLog(log) {}

    ~window_hook();

    bool hookWindow(HWND hwnd);
    void unhookWindow();

    template<typename T>
    void setHandler(const T& h)
    {
        mHandler = h;
    }
};

#endif // WINDOW_HOOK_HPP

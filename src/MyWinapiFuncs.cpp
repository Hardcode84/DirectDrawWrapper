#include "MyWinapiFuncs.hpp"

#include <tuple>
#include <type_traits>
#include "tools/func_hook.hpp"

#include "MyDirectDrawSw.hpp"

template<typename FUNC>
struct HookInfo
{
    const char* moduleName = nullptr;
    const char* funcName   = nullptr;
    FUNC* original         = nullptr;
    const FUNC* hook       = nullptr;
    HookInfo(const char* m, const char* f, const FUNC* h):
        moduleName(m), funcName(f), hook(h) {}
};

auto GetCursorPosHook   = HookInfo<BOOL WINAPI(LPPOINT)>      ("User32.dll", "GetCursorPos",   &MyGetCursorPos);
auto ClientToScreenHook = HookInfo<BOOL WINAPI(HWND ,LPPOINT)>("User32.dll", "ClientToScreen", &MyClientToScreen);

auto Hooks = std::make_tuple(&GetCursorPosHook,
                             &ClientToScreenHook);

template<typename FUNC>
bool InitHook(HookInfo<FUNC>& hook, logger* mLog, HMODULE module)
{
    const char* moduleName = hook.moduleName;
    const char* funcName   = hook.funcName;
    if(nullptr == hook.original)
    {
        const HRESULT hr = func_hook::PatchIat(module,
                                               moduleName,
                                               funcName,
                                               hook.hook,
                                               &hook.original);
        if(FAILED(hr))
        {
            LOG_ERROR() << "Unable to set hook \"" << funcName << "\": " << getWinErrorFromHr(hr);
        }
    }
    return true;
}

template<typename FUNC>
void ReleaseHook(HookInfo<FUNC>& hook, logger* mLog, HMODULE module)
{
    const char* moduleName = hook.moduleName;
    const char* funcName   = hook.funcName;
    if(nullptr != hook.original)
    {
        const HRESULT hr = func_hook::PatchIat(module,
                                               moduleName,
                                               funcName,
                                               hook.original);
        if(FAILED(hr))
        {
            LOG_ERROR() << "Unable to release hook \"" << funcName << "\": " << hr;
        }
        hook.original = nullptr;
    }
}

template<unsigned I,typename... Types>
typename std::enable_if<(I < sizeof...(Types)), bool>::type
IterateTupleInit(std::tuple<Types...>& t, logger* log, HMODULE module)
{
    if(InitHook(*std::get<I>(t), log, module))
    {
        return IterateTupleInit<I + 1>(t, log, module);
    }
    return false;
}

template<unsigned I,typename... Types>
typename std::enable_if<(I >= sizeof...(Types)), bool>::type
IterateTupleInit(std::tuple<Types...>&, logger*, HMODULE)
{
    return true;
}

template<unsigned I,typename... Types>
typename std::enable_if<(I < sizeof...(Types))>::type
IterateTupleRelease(std::tuple<Types...>& t, logger* log, HMODULE module)
{
    ReleaseHook(*std::get<I>(t), log, module);
    IterateTupleRelease<I + 1>(t, log, module);
}

template<unsigned I,typename... Types>
typename std::enable_if<(I >= sizeof...(Types))>::type
IterateTupleRelease(std::tuple<Types...>&, logger*, HMODULE)
{
}

bool CreateHooks(logger* mLog)
{
    LOG_STATIC_FUNCTION();
    HMODULE currModule = GetModuleHandle(nullptr);
    if(nullptr == currModule)
    {
        LOG_ERROR() << "GetModuleHandle failed: " << getWinError();
        return false;
    }
    return IterateTupleInit<0>(Hooks, mLog, currModule);
}

void RemoveHooks(logger* mLog)
{
    LOG_STATIC_FUNCTION();
    HMODULE currModule = GetModuleHandle(nullptr);
    if(nullptr == currModule)
    {
        LOG_ERROR() << "GetModuleHandle failed: " << getWinError();
        return;
    }
    IterateTupleRelease<0>(Hooks, mLog, currModule);
}



BOOL WINAPI MyGetCursorPos(LPPOINT lpPoint)
{
    const auto result = GetCursorPosHook.original(lpPoint);
    if(result)
    {
        const MyDirectDrawSw* inst = MyDirectDrawSw::getInstance();
        if(nullptr != inst)
        {
            const auto mLog = inst->getLogger();
            const auto offset = inst->getWindowPos(true);
            LOG_INFO() << "MyGetCursorPos: " << offset.first << "-" << offset.second;
            lpPoint->x -= offset.first;
            lpPoint->y -= offset.second;
        }
    }
    return result;
}

BOOL WINAPI MyClientToScreen(HWND hWnd, LPPOINT lpPoint)
{
    const auto result = ClientToScreenHook.original(hWnd, lpPoint);
    if(result)
    {
        const MyDirectDrawSw* inst = MyDirectDrawSw::getInstance();
        if((nullptr != inst) && (hWnd == inst->getHWnd()))
        {
            const auto mLog = inst->getLogger();
            const auto offset = inst->getWindowPos(true);
            LOG_INFO() << "MyClientToScreen: " << offset.first << "-" << offset.second;
            lpPoint->x -= offset.first;
            lpPoint->y -= offset.second;
        }
    }
    return result;
}


#include "libmain.hpp"

#include "simple_logger/logger.hpp"
#include "MyDirectDrawProxy.hpp"
#include "MyDirectDrawSw.hpp"
#include "Settings.hpp"
#include "MyWinapiFuncs.hpp"

logger* mLog = nullptr;
const Settings* settings = nullptr;

HINSTANCE           hOriginalDll  = nullptr;
HINSTANCE           hThisInstance = nullptr;

typedef HRESULT (WINAPI* DirectDrawCreate_Type)(GUID *, LPDIRECTDRAW *, IUnknown *);
typedef HRESULT (WINAPI* DirectDrawEnumerateW_Type)(LPDDENUMCALLBACKW, LPVOID);
typedef HRESULT (WINAPI* DirectDrawEnumerateA_Type)(LPDDENUMCALLBACKA, LPVOID);
typedef HRESULT (WINAPI* DirectDrawEnumerateExW_Type)(LPDDENUMCALLBACKEXW, LPVOID, DWORD);
typedef HRESULT (WINAPI* DirectDrawEnumerateExA_Type)(LPDDENUMCALLBACKEXA, LPVOID, DWORD);

DirectDrawCreate_Type DirectDrawCreate_fn = nullptr;
DirectDrawEnumerateW_Type DirectDrawEnumerateW_fn = nullptr;
DirectDrawEnumerateA_Type DirectDrawEnumerateA_fn = nullptr;
DirectDrawEnumerateExW_Type DirectDrawEnumerateExW_fn = nullptr;
DirectDrawEnumerateExA_Type DirectDrawEnumerateExA_fn = nullptr;

#ifdef __GNUC__
extern "C" DDRAW_EXPORT
#endif
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    (void)lpReserved;
    bool result = true;
    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: result = InitInstance(hModule); break;
        case DLL_PROCESS_DETACH: ExitInstance(); break;

        case DLL_THREAD_ATTACH:  break;
        case DLL_THREAD_DETACH:  break;
    }
    return result;
}

HRESULT WINAPI DirectDrawCreate(GUID *lpGUID, LPDIRECTDRAW *lplpDD, IUnknown *pUnkOuter)
{
    LOG_STATIC_FUNCTION();
    *lplpDD = nullptr;
    if(Settings::modeSW == settings->mode())
    {
        const auto ret = MyDirectDrawSw::getInstance(mLog, settings);
        if(nullptr != ret)
        {
            ret->AddRef();
            *lplpDD = ret;
            return S_OK;
        }
        else
        {
            return E_FAIL;
        }
    }

    if(!hOriginalDll)
    {
        if(!LoadOriginalDll())
        {
            return E_FAIL;
        }
    }

    LPDIRECTDRAW src;
    const HRESULT result = (DirectDrawCreate_fn(lpGUID, &src, pUnkOuter));
    if(FAILED(result))
    {
        return result;
    }

    switch(settings->mode())
    {
        case Settings::modeClear:
            *lplpDD = src;
            break;
        case Settings::modeLogger:
            *lplpDD = new MyDirectDrawProxy(mLog, src);
            break;
        default:
            LOG_ERROR() << "Unknown mode: " << settings->mode();
            src->Release();
            return E_FAIL;
    }

    return result;
}

HRESULT WINAPI DirectDrawCreateEx(GUID *lpGuid, LPVOID  *lplpDD, REFIID  iid,IUnknown *pUnkOuter)
{
    if(iid != IID_IDirectDraw7)
    {
        return DDERR_INVALIDPARAMS;
    }
    return DirectDrawCreate(lpGuid, (LPDIRECTDRAW *)lplpDD, pUnkOuter);
}

HRESULT WINAPI DirectDrawEnumerateW(LPDDENUMCALLBACKW lpCallback, LPVOID lpContext)
{
    LOG_STATIC_FUNCTION();
    if(!hOriginalDll)
    {
        if(!LoadOriginalDll())
        {
            return E_FAIL;
        }
    }

    return DirectDrawEnumerateW_fn(lpCallback, lpContext);
}

HRESULT WINAPI DirectDrawEnumerateA(LPDDENUMCALLBACKA lpCallback, LPVOID lpContext)
{
    LOG_STATIC_FUNCTION();
    if(!hOriginalDll)
    {
        if(!LoadOriginalDll())
        {
            return E_FAIL;
        }
    }
    return DirectDrawEnumerateA_fn(lpCallback, lpContext);
}

HRESULT WINAPI DirectDrawEnumerateExW(LPDDENUMCALLBACKEXW lpCallback, LPVOID lpContext, DWORD dwFlags)
{
    LOG_STATIC_FUNCTION();
    if(!hOriginalDll)
    {
        if(!LoadOriginalDll())
        {
            return E_FAIL;
        }
    }

    return DirectDrawEnumerateExW_fn(lpCallback, lpContext, dwFlags);
}

HRESULT WINAPI DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags)
{
    LOG_STATIC_FUNCTION();
    if(!hOriginalDll)
    {
        if(!LoadOriginalDll())
        {
            return E_FAIL;
        }
    }

    return DirectDrawEnumerateExA_fn(lpCallback, lpContext, dwFlags);
}
/*
VOID WINAPI AcquireDDThreadLock(void)
{
    LOG_STATIC_FUNCTION();
}

VOID WINAPI ReleaseDDThreadLock(void)
{
    LOG_STATIC_FUNCTION();
}

DWORD WINAPI D3DParseUnknownCommand(LPVOID lpCmd, LPVOID *lpRetCmd)
{
    return 0;
}

HRESULT WINAPI DllCanUnloadNow(void)
{
    LOG_FUNCTION_E();
}

HRESULT WINAPI DllGetClassObject (const CLSID &rclsid, const IID &riid, void **ppv)
{
    LOG_FUNCTION_E();
}
*/
bool InitInstance(HANDLE hModule)
{
    hOriginalDll  = nullptr;
    hThisInstance = (HINSTANCE)hModule;

    if(nullptr == settings)
    {
        settings = new Settings("./ddraw.ini");
    }
    if(nullptr == mLog)
    {
        mLog = new logger(settings->logLevel(), false, true);
        mLog->setFile("./debug.log");
    }
    LOG_INFO() << FUNC_NAME;
    return true;
}

bool LoadOriginalDll(void)
{
    LOG_STATIC_FUNCTION();
    char buffer[MAX_PATH];

    ::GetSystemDirectory(buffer,MAX_PATH);
    strcat(buffer,"/ddraw.dll");

    if(nullptr == hOriginalDll) hOriginalDll = ::LoadLibrary(buffer);
    if(nullptr == hOriginalDll)
    {
        LOG_ERROR() << "LoadLibrary failed: " << getWinError();
        return false;
    }

    const std::pair<const char*, FARPROC*> funcs[] = {{"DirectDrawCreate",       (FARPROC*)&DirectDrawCreate_fn},
                                                      {"DirectDrawEnumerateW",   (FARPROC*)&DirectDrawEnumerateW_fn},
                                                      {"DirectDrawEnumerateA",   (FARPROC*)&DirectDrawEnumerateA_fn},
                                                      {"DirectDrawEnumerateExW", (FARPROC*)&DirectDrawEnumerateExW_fn},
                                                      {"DirectDrawEnumerateExA", (FARPROC*)&DirectDrawEnumerateExA_fn}};
    for(const auto& i: funcs)
    {
        const FARPROC res = ::GetProcAddress(hOriginalDll, i.first);
        if(nullptr == res)
        {
            LOG_ERROR() << "Unable to get \"" << i.first << "\": " << getWinError();
            return false;
        }
        *i.second = res;
    }
    return true;
}

void ExitInstance()
{
    LOG_INFO() << FUNC_NAME;
    RemoveHooks(mLog);
    MyDirectDrawSw::ReleaseInstance();
    if(nullptr != hOriginalDll)
    {
        ::FreeLibrary(hOriginalDll);
        hOriginalDll = nullptr;
    }
    hThisInstance = nullptr;

    delete mLog;
    mLog = nullptr;
    delete settings;
    settings = nullptr;
}


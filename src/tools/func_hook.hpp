#ifndef FUNC_HOOK_HPP
#define FUNC_HOOK_HPP

// Import Address Table patcher from:
// http://jpassing.com/2008/01/06/using-import-address-table-hooking-for-testing/

#include <type_traits>
#include <windows.h>

#include "help_funcs.hpp"

struct func_hook
{
    template<typename FUNC>
    static HRESULT PatchIat(
       HMODULE Module,
       const char* ImportedModuleName,
       const char* ImportedProcName,
       FUNC* AlternateProc,
       FUNC** OldProc = (FUNC**)nullptr)
    {
        //static_assert(std::is_function<FUNC>::value,"Not a function");
        return PatchIatInternal(Module,
                                ImportedModuleName,
                                ImportedProcName,
                                help_funcs::union_cast<void*>(AlternateProc),
                                help_funcs::union_cast<void**>(OldProc));
    }

    private:

    static HRESULT PatchIatInternal(
       HMODULE Module,
       const char* ImportedModuleName,
       const char* ImportedProcName,
       void* AlternateProc,
       void** OldProc
  );

};
#endif // FUNC_HOOK_HPP

#include "func_hook.hpp"

#include <cstring>

#define PtrFromRva( base, rva ) ( ( ( PBYTE ) base ) + rva )

HRESULT func_hook::PatchIatInternal(HMODULE Module,
   const char* ImportedModuleName,
   const char* ImportedProcName,
   void* AlternateProc,
   void** OldProc
  )
{
  PIMAGE_DOS_HEADER DosHeader = ( PIMAGE_DOS_HEADER ) Module;
  PIMAGE_NT_HEADERS NtHeader;
  PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor;
  UINT Index;

  NtHeader = ( PIMAGE_NT_HEADERS )
    PtrFromRva( DosHeader, DosHeader->e_lfanew );
  if( IMAGE_NT_SIGNATURE != NtHeader->Signature )
  {
    return HRESULT_FROM_WIN32( ERROR_BAD_EXE_FORMAT );
  }

  ImportDescriptor = ( PIMAGE_IMPORT_DESCRIPTOR )
    PtrFromRva( DosHeader,
      NtHeader->OptionalHeader.DataDirectory
        [ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress );

  //
  // Iterate over import descriptors/DLLs.
  //
  for ( Index = 0;
        ImportDescriptor[ Index ].Characteristics != 0;
        Index++ )
  {
    PSTR dllName = ( PSTR )
      PtrFromRva( DosHeader, ImportDescriptor[ Index ].Name );

    if ( 0 == strcasecmp( dllName, ImportedModuleName ) )
    {
      //
      // This the DLL we are after.
      //
      PIMAGE_THUNK_DATA Thunk;
      PIMAGE_THUNK_DATA OrigThunk;

      if ( ! ImportDescriptor[ Index ].FirstThunk ||
         ! ImportDescriptor[ Index ].OriginalFirstThunk )
      {
        return E_INVALIDARG;
      }

      Thunk = ( PIMAGE_THUNK_DATA )
        PtrFromRva( DosHeader,
          ImportDescriptor[ Index ].FirstThunk );
      OrigThunk = ( PIMAGE_THUNK_DATA )
        PtrFromRva( DosHeader,
          ImportDescriptor[ Index ].OriginalFirstThunk );

      for ( ; OrigThunk->u1.Function != 0;
              OrigThunk++, Thunk++ )
      {
        if ( OrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG )
        {
          //
          // Ordinal import - we can handle named imports
          // ony, so skip it.
          //
          continue;
        }

        PIMAGE_IMPORT_BY_NAME import = ( PIMAGE_IMPORT_BY_NAME )
          PtrFromRva( DosHeader, OrigThunk->u1.AddressOfData );

        OutputDebugStringA(( const char* ) import->Name);
        if ( 0 == strcmp( ImportedProcName,
                              ( const char* ) import->Name ) )
        {
          //
          // Proc found, patch it.
          //
          DWORD junk;
          MEMORY_BASIC_INFORMATION thunkMemInfo;

          //
          // Make page writable.
          //
          VirtualQuery(
            Thunk,
            &thunkMemInfo,
            sizeof( MEMORY_BASIC_INFORMATION ) );
          if ( ! VirtualProtect(
            thunkMemInfo.BaseAddress,
            thunkMemInfo.RegionSize,
            PAGE_EXECUTE_READWRITE,
            &thunkMemInfo.Protect ) )
          {
            return HRESULT_FROM_WIN32( GetLastError() );
          }

          //
          // Replace function pointers (non-atomically).
          //
          if ( OldProc )
          {
            *OldProc = ( PVOID ) ( DWORD_PTR )
                Thunk->u1.Function;
          }
#ifdef _WIN64
          Thunk->u1.Function = ( ULONGLONG ) ( DWORD_PTR )
              AlternateProc;
#else
          Thunk->u1.Function = ( DWORD ) ( DWORD_PTR )
              AlternateProc;
#endif
          //
          // Restore page protection.
          //
          if ( ! VirtualProtect(
            thunkMemInfo.BaseAddress,
            thunkMemInfo.RegionSize,
            thunkMemInfo.Protect,
            &junk ) )
          {
            return HRESULT_FROM_WIN32( GetLastError() );
          }

          return S_OK;
        }
      }

      //
      // Import not found.
      //
      return HRESULT_FROM_WIN32( ERROR_PROC_NOT_FOUND );
    }
  }

  //
  // DLL not found.
  //
  return HRESULT_FROM_WIN32( ERROR_MOD_NOT_FOUND );
}

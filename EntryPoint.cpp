#include "Common.hpp"
#include "GlobalVars.hpp"
#include "D3D.hpp"

DWORD WINAPI MainThread( PVOID pModule )
{
    if ( MH_Initialize( ) != MH_OK )
        return 0ul;

    AllocConsole( );

    freopen_s( ( FILE** )stdin, "conin$", "r", stdin );
    freopen_s( ( FILE** )stdout, "conout$", "w", stdout );
    freopen_s( ( FILE** )stderr, "conout$", "w", stderr );

    SetConsoleTitleA( "DX9-Cheat-Base by Valee1337" );

    if ( !D3D::AtttachHooks( ) )
    {
        printf_s( "Failed D3D::AtttachHooks( ).\n" );
        return 0ul;
    }

    return 1ul;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD   dwReason,
                       LPVOID  lpReserved
)
{
    if ( dwReason == DLL_PROCESS_ATTACH )
    {
        Beep( 200, 200 );

        DisableThreadLibraryCalls( hModule );

        const HANDLE hMainThread = CreateThread( nullptr, NULL, MainThread, hModule, NULL, nullptr );
        if ( hMainThread != nullptr )
        {
            CloseHandle( hMainThread );
        }
    }

    return TRUE;
}

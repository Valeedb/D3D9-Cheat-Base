#include "Common.hpp"
#include "GlobalVars.hpp"
#include "D3D.hpp"
#include "SDK.hpp"

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

    SDK::FindOffsets( );
    printf_s( "Scanned for offsets.\n" );

    return 1ul;
}

DWORD WINAPI UnloadThread( PVOID pModule )
{
    while ( !GetAsyncKeyState( VK_END ) )
        std::this_thread::sleep_for( 1s );

    printf_s( "Goodbye!\n" );

    std::this_thread::sleep_for( 1s );
    g_bUnloading = true;

    if ( g_pDefaultFont != nullptr )
    {
        g_pDefaultFont->Release( );
        g_pDefaultFont = nullptr;
    }

    // This will take care of all hooks instead of doing it for each one
    MH_DisableHook( MH_ALL_HOOKS );
    MH_RemoveHook( MH_ALL_HOOKS );
    MH_Uninitialize( );

    FreeConsole( );
    fclose( stdin );
    fclose( stdout );
    fclose( stderr );

    Beep( 200, 200 );

    FreeLibraryAndExitThread( ( HMODULE )pModule, 1ul );

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

        const HANDLE hUnloadThread = CreateThread( nullptr, NULL, UnloadThread, hModule, NULL, nullptr );
        if ( hUnloadThread != nullptr )
        {
            CloseHandle( hUnloadThread );
        }
    }

    return TRUE;
}

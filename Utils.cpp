#include "Utils.hpp"

uint8_t* Utils::FindPattern( HMODULE hHandle, const char* szPattern, const char* szMask )
{
    if ( hHandle == nullptr )
    {
        return nullptr;
    }

    MODULEINFO moduleInfo;
    if ( GetModuleInformation( GetCurrentProcess( ), hHandle, &moduleInfo, sizeof( moduleInfo ) ) )
    {
        uint8_t* pBaseAddress = static_cast< uint8_t* >( moduleInfo.lpBaseOfDll );
        size_t uModuleSize = moduleInfo.SizeOfImage;

        for ( size_t i = 0; i < uModuleSize; ++i )
        {
            bool bFound = true;
            for ( size_t j = 0; szMask[ j ] != '\0'; ++j )
            {
                if ( szMask[ j ] == 'x' && szPattern[ j ] != pBaseAddress[ i + j ] )
                {
                    bFound = false;
                    break;
                }
            }

            if ( bFound )
            {
                return pBaseAddress + i;
            }
        }
    }

    MessageBoxA( nullptr, 
                 std::format( 
                     "Failed to find pattern {} with mask {}.", 
                     szPattern,
                     szMask 
                 ).c_str( ),
                 "DX9-Cheat-Base",
                 MB_OK | MB_ICONERROR );

    return nullptr;
}

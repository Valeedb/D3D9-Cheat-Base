#include "D3D.hpp"
#include "GlobalVars.hpp"

typedef HRESULT( __stdcall* EndScene_t )( IDirect3DDevice9* );
EndScene_t oEndScene = nullptr;
static HRESULT __stdcall Hooked_EndScene( IDirect3DDevice9* pDevice )
{
	// the magic? we steal it from the game now
	if ( g_pD3Device == nullptr )
	{
		g_pD3Device = pDevice;
		printf_s( "Captured D3DDevice at 0x%p\n", g_pD3Device );
	}

	// ...
	// ...
	// ...

	return oEndScene( pDevice );
}

typedef HRESULT( __stdcall* Reset_t )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
Reset_t oReset = nullptr;
static HRESULT __stdcall Hooked_Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParams )
{
	// ...
	// ...
	// ...

	return oReset( pDevice, pPresentParams );
}

bool D3D::AtttachHooks( HWND hWnd )
{
	IDirect3D9* pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( pD3D == nullptr )
		return false;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd == nullptr ? GetForegroundWindow( ) : hWnd;

	IDirect3DDevice9* pD3DeviceTemp = nullptr;

	const HRESULT hResult = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
												D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DeviceTemp );
	if ( FAILED( hResult ) )
	{
		printf_s( "Failed to get create temp D3DDevice9. Trying in full-screan...\n" );

		// try again, maybe it was in full-screan
		d3dpp.Windowed = FALSE;

		const HRESULT hResultFinal = pD3D->CreateDevice( D3DADAPTER_DEFAULT,
														 D3DDEVTYPE_HAL,
														 d3dpp.hDeviceWindow,
														 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
														 &d3dpp,
														 &pD3DeviceTemp );

		if ( FAILED( hResultFinal ) )
		{
			pD3D->Release( );
			return false;
		}
	}

	if ( pD3DeviceTemp == nullptr )
	{
		pD3D->Release( );
		return false;
	}

	printf_s( "Created temp D3DDevice9 at 0x%p.\n", pD3DeviceTemp );

	void** pVTable = *reinterpret_cast< void*** >( pD3DeviceTemp );

	MH_CreateHook( pVTable[ 42 ], &Hooked_EndScene, reinterpret_cast< void** >( &oEndScene ) );
	MH_CreateHook( pVTable[ 16 ], &Hooked_Reset, reinterpret_cast< void** >( &oReset ) );

	// enable EndScene and Reset hooks instead of all hooks.
	MH_EnableHook( pVTable[ 42 ] );
	MH_EnableHook( pVTable[ 16 ] );

	printf_s( "Attached D3D9 hooks.\n" );

	pD3DeviceTemp->Release( );
	pD3D->Release( );

	return true;
}

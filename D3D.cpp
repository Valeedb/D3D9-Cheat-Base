#include "D3D.hpp"
#include "GlobalVars.hpp"

ID3DXFont* g_pDefaultFont = nullptr;

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

	if ( g_pDefaultFont == nullptr )
	{
		D3DXCreateFont( g_pD3Device, 14, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pDefaultFont );
		printf_s( "Created g_pDefaultFont at 0x%p\n", g_pDefaultFont );
	}

	D3D::DrawString( { 10.f, 10.f }, D3DCOLOR_RGBA( 255, 255, 0, 255 ), "DX9-Cheat-Base" );
	D3D::DrawString( { 10.f, 24.f }, D3DCOLOR_RGBA( 0, 255, 255, 255 ), "By Valee1337", false );

	return oEndScene( pDevice );
}

typedef HRESULT( __stdcall* Reset_t )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
Reset_t oReset = nullptr;
static HRESULT __stdcall Hooked_Reset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentParams )
{
	if ( g_pDefaultFont )
	{
		g_pDefaultFont->OnLostDevice( );
	}

	HRESULT result = oReset( pDevice, pPresentParams );

	if ( g_pDefaultFont )
	{
		g_pDefaultFont->OnResetDevice( );
	}
	else
	{
		D3DXCreateFont( g_pD3Device, 14, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &g_pDefaultFont );
	}

	return result;
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

void D3D::DrawString( Vec2 vecPos, D3DCOLOR colColor, const char* szText, bool bOutlined, bool bCenetered, 
					  ID3DXFont* pFont )
{
	RECT rect = { 
		( LONG )vecPos.x, 
		( LONG )vecPos.y, 
		( LONG )( vecPos.x + 500 ),
		( LONG )( vecPos.y + 50 ) 
	};

	if ( bOutlined )
	{
		D3DCOLOR outlineColor = D3DCOLOR_ARGB( 255, 0, 0, 0 );
		RECT outlineRect = rect;

		outlineRect.left--;
		outlineRect.top--;
		pFont->DrawTextA( nullptr, szText, -1, &outlineRect, DT_LEFT | DT_WORDBREAK, outlineColor );

		outlineRect.left += 2;
		outlineRect.top++;
		pFont->DrawTextA( nullptr, szText, -1, &outlineRect, DT_LEFT | DT_WORDBREAK, outlineColor );

		outlineRect.left--;
		outlineRect.top++;
		pFont->DrawTextA( nullptr, szText, -1, &outlineRect, DT_LEFT | DT_WORDBREAK, outlineColor );

		outlineRect.left--;
		outlineRect.top--;
		pFont->DrawTextA( nullptr, szText, -1, &outlineRect, DT_LEFT | DT_WORDBREAK, outlineColor );
	}

	pFont->DrawTextA( nullptr, szText, -1, &rect, bCenetered ? DT_CENTER : DT_LEFT | DT_WORDBREAK, colColor );
}

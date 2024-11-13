#pragma once
#include "Common.hpp"
#include "Utils.hpp"

extern ID3DXFont* g_pDefaultFont;

namespace D3D
{
	bool AtttachHooks( HWND hWnd = nullptr );
	void DrawString( Vec2 vecPos, Color colColor, const char* szText, bool bOutlined = true,
					 bool bCenetered = false, ID3DXFont* pFont = g_pDefaultFont );
}

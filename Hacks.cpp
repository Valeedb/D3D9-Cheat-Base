#include "Hacks.hpp"

void Hacks::OnRender( )
{
	// Watermark
	D3D::DrawString( { 10.f, 10.f }, { 83U, 230U, 79U, 255U }, "DX9 Cheat Base" );
}

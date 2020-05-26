#include "CDrawManager.h"
#include <mutex>
//===================================================================================
CDrawManager gDrawManager;

#define ESP_HEIGHT 14
//===================================================================================
void CDrawManager::Initialize( )
{
	if ( gInts.Surface == NULL )
		return;

	gInts.Engine->GetScreenSize( gScreenSize.iScreenWidth, gScreenSize.iScreenHeight );
	m_Font = gInts.Surface->CreateFont( );
	gInts.Surface->SetFontGlyphSet( m_Font, "Arial", ESP_HEIGHT, 500, 0, 0, 0x200 );
}
//===================================================================================
void CDrawManager::DrawLine(int x, int y, int x1, int y1, Color clrColor)
{
	gInts.Surface->DrawSetColor(clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a());
	gInts.Surface->DrawLine(x, y, x1, y1);
}
//===================================================================================
void CDrawManager::DrawString(int x, int y, Color color, HFont font, const wchar_t *pszText)
{
	if( pszText == NULL )
		return;

	gInts.Surface->DrawSetTextPos( x, y );
	gInts.Surface->DrawSetTextFont(font);
	gInts.Surface->DrawSetTextColor(color.r(), color.g(), color.b(), color.a());
	gInts.Surface->DrawPrintText( pszText, wcslen( pszText ) );
}
//===================================================================================

//Thank you kolo.
std::wstring CDrawManager::StringToWide(const std::string& text)
{
	std::wstring wide(text.length(), L' ');
	std::copy(text.begin(), text.end(), wide.begin());

	return wide;
}

int CDrawManager::GetWidth(unsigned long font, const char* input)
{
	INT iWide = 0;
	INT iTall = 0;
	INT iBufSize = MultiByteToWideChar(CP_UTF8, 0x0, input, -1, NULL, 0);

	wchar_t* pszUnicode = new wchar_t[iBufSize];

	MultiByteToWideChar(CP_UTF8, 0x0, input, -1, pszUnicode, iBufSize);

	gInts.Surface->GetTextSize(font, pszUnicode, iWide, iTall);

	delete[] pszUnicode;

	return iWide;
}

void CDrawManager::DrawStringA(unsigned long font, bool center, int x, int y, int r, int g, int b, int a, const char *input, ...)
{
	CHAR szBuffer[MAX_PATH];

	if (!input)
		return;

	vsprintf(szBuffer, input, (char*)&input + _INTSIZEOF(input));

	if (center)
		x -= GetWidth(font, szBuffer) / 2;

	gInts.Surface->DrawSetTextColor(r, g, b, a);
	gInts.Surface->DrawSetTextFont(font);
	gInts.Surface->DrawSetTextPos(x, y);
	std::wstring wide = StringToWide(std::string(szBuffer));
	gInts.Surface->DrawPrintText(wide.c_str(), wide.length());
}

//===================================================================================
void CDrawManager::DrawT(int X, int Y, Color Color, HFont Font, bool Center, const char* _Input, ...)
{
	char Buffer[2048] = { '\0' };
	va_list Args;

	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];

	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	int Width = 0, Height = 0;

	if (Center)
		gInts.Surface->GetTextSize(Font, WideBuffer, Width, Height);

	gInts.Surface->DrawSetTextColor(Color.r(), Color.g(), Color.b(), Color.a());
	gInts.Surface->DrawSetTextFont(Font);
	gInts.Surface->DrawSetTextPos(X, Y);
	gInts.Surface->DrawPrintText(WideBuffer, wcslen(WideBuffer));
}
//===================================================================================
byte CDrawManager::GetESPHeight( )
{
	return ESP_HEIGHT;
}
//===================================================================================
void CDrawManager::DrawRect(int x, int y, int w, int h, Color clrColor)
{
	gInts.Surface->DrawSetColor(clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a());
	gInts.Surface->DrawFilledRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::OutlineRect(int x, int y, int w, int h, Color clrColor)
{
	gInts.Surface->DrawSetColor(clrColor.r(), clrColor.g(), clrColor.b(), clrColor.a());
	gInts.Surface->DrawOutlinedRect(x, y, x + w, y + h);
}
//===================================================================================
void CDrawManager::DrawBox( Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius )
{
	Vector vScreen;

	if( !WorldToScreen( vOrigin, vScreen ) )
		return;

	int radius2 = radius<<1;

	OutlineRect( vScreen.x - radius + box_width, vScreen.y - radius + box_width, radius2 - box_width, radius2 - box_width, 0x000000FF );
	OutlineRect( vScreen.x - radius - 1, vScreen.y - radius - 1, radius2 + ( box_width + 2 ), radius2 + ( box_width + 2 ), 0x000000FF );
	DrawRect( vScreen.x - radius + box_width, vScreen.y - radius, radius2 - box_width, box_width,COLORCODE( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y + radius, radius2, box_width,COLORCODE( r, g, b, alpha ));
	DrawRect( vScreen.x - radius, vScreen.y - radius, box_width, radius2,COLORCODE( r, g, b, alpha ));
	DrawRect( vScreen.x + radius, vScreen.y - radius, box_width, radius2 + box_width, COLORCODE( r, g, b, alpha ) );
}
//===================================================================================
bool CDrawManager::WorldToScreen(const Vector &vOrigin, Vector &vScreen)
{
	return (gInts.DebugOverlay->ScreenPosition(vOrigin, vScreen) != 1);
}

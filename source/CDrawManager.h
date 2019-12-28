#pragma once
//===================================================================================
#include "SDK.h"
#include "Panels.h"

#define RED(COLORCODE)	((int) ( COLORCODE >> 24) )
#define BLUE(COLORCODE)	((int) ( COLORCODE >> 8 ) & 0xFF )
#define GREEN(COLORCODE)	((int) ( COLORCODE >> 16 ) & 0xFF )
#define ALPHA(COLORCODE)	((int) COLORCODE & 0xFF )
#define COLORCODE(r,g,b,a)((DWORD)((((r)&0xff)<<24)|(((g)&0xff)<<16)|(((b)&0xff)<<8)|((a)&0xff)))

enum FontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};


//===================================================================================
class CDrawManager
{
public:
	void Initialize( );
	void DrawLine(int x, int y, int x1, int y1, Color clrColor);
	void DrawString(int x, int y, Color color, HFont font, const wchar_t * pszText);
	void DrawString(int x, int y, Color color, HFont font, const char * pszText, ...);
	void DrawT(int X, int Y, Color Color, HFont Font, bool Center, const char * _Input, ...);
	byte GetESPHeight( );
	void DrawRect(int x, int y, int w, int h, Color clrColor);
	void OutlineRect(int x, int y, int w, int h, Color clrColor);
	int GetPixelTextSize ( const char *pszText );
	int GetPixelTextSize ( wchar_t *pszText );
	void DrawBox( Vector vOrigin, int r, int g, int b, int alpha, int box_width, int radius );
	bool WorldToScreen( Vector &vOrigin, Vector &vScreen );
	DWORD dwGetTeamColor( int iIndex )
	{
		static DWORD dwColors[] = { 0, //Dummy
					 0, // 1 Teamone (UNUSED)
					 0xFF8000FF, // 2 Teamtwo (RED)
					 0x0080FFFF, // 3 teamthree (BLUE)
					 0 // 4 Teamfour (UNUSED) 
					 };
		return dwColors[ iIndex ];
	}
private:
	unsigned long m_Font;

};
//===================================================================================
extern CDrawManager gDrawManager;
//===================================================================================
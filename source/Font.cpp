#include "Font.h"

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

void Font::Create()
{
	MenuText = gInts.Surface->CreateFont();
	ESP = gInts.Surface->CreateFont();
	Name = gInts.Surface->CreateFont();

	gInts.Surface->SetFontGlyphSet(MenuText, "Tahoma", 11, 500, 0, 0, FONTFLAG_OUTLINE);
	gInts.Surface->SetFontGlyphSet(ESP, "Arial", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);
	gInts.Surface->SetFontGlyphSet(Name, "Tahoma", 12, 700, 0, 0, FONTFLAG_DROPSHADOW);
}
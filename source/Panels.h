#pragma once
#include "VMT.h"

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

typedef void(__thiscall* PaintTraverse_t) (PVOID, unsigned int, bool, bool); 
extern PaintTraverse_t oPaintTraverse;
void __fastcall Hooked_PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();

extern CScreenSize gScreenSize;
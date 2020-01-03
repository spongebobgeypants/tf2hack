#pragma once
#include "VMT.h"

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

extern std::unique_ptr<VMTHook> panelHook;
void __fastcall Hooked_PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();

extern CScreenSize gScreenSize;
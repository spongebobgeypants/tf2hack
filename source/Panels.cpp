#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "CNetvarManager.h"
#include "Globals.h"
#include "CMiscellaneous.h"
#include "CVisuals.h"

CScreenSize gScreenSize;
std::unique_ptr<VMTHook> panelHook;
typedef void(__thiscall* PaintTraverse_t) (PVOID, unsigned int, bool, bool);

//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	static auto oPaintTraverse = panelHook->GetOriginal<PaintTraverse_t>(41);

	if (!strcmp("HudScope", gInts.Panels->GetName(vguiPanel)) && gMenu::Visuals::RemoveScope) // Remove scope.
	{
		return;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int vguiMatSystemTopPanel;


	if (vguiMatSystemTopPanel == NULL)
	{
		const char* szName = gInts.Panels->GetName(vguiPanel);
		if (szName[0] == 'M' && szName[3] == 'S') //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
		{
			vguiMatSystemTopPanel = vguiPanel;
			Intro();
		}
	}

	if (vguiMatSystemTopPanel == vguiPanel) //If we're on MatSystemTopPanel, call our drawing code.
	{

		if (gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible() || ((GetAsyncKeyState(VK_F12) || gInts.Engine->IsTakingScreenshot())))
		{
			return; //We don't want to draw at the menu.
		}

		CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me);  //Grab the local player's entity.

		if (pBaseLocalEnt == NULL) //Always check for null pointers.
			return;

		gVisuals::Do(pBaseLocalEnt);
		gMiscellaneous::BypassPure();
	}

}
//===================================================================================
void Intro( void )
{
	gDrawManager.Initialize(); //Initalize the drawing class.
	gNetvars = std::make_unique<NetvarTree>(); //Initialize the netvars
}
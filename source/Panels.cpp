#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "CNetvarManager.h"
#include "Globals.h"
#include "CMiscellaneous.h"
#include "CVisuals.h"

CScreenSize gScreenSize;

PaintTraverse_t oPaintTraverse = nullptr;

//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	
	if (!strcmp("HudScope", gInts.Panels->GetName(vguiPanel)) && gMenu::Visuals::RemoveScope) // Remove scope.
	{
		return;
	}

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
	static unsigned int vguiFocusOverlayPanel;

	if (vguiFocusOverlayPanel == NULL)
	{											//FocusOverlayPanel
		const char* szName = gInts.Panels->GetName(vguiPanel);
		if (szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P')
		{
			vguiFocusOverlayPanel = vguiPanel;
			Intro();
		}
	}


	if (vguiFocusOverlayPanel == vguiPanel)
	{
		if (gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible() || gInts.Engine->IsTakingScreenshot())
		{
			return; //We don't want to draw at the menu.
		}

		CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me);  //Grab the local player's entity.

		if (pBaseLocalEnt == NULL) //Always check for null pointers.
			return;

		gVisuals::Do(pBaseLocalEnt);
		gMiscellaneous::BypassPure();

		gInts.Panels->SetMouseInputEnabled(vguiPanel, g::b_MenuEnable);
	}

}
//===================================================================================
void Intro( void )
{
	gDrawManager.Initialize(); //Initalize the drawing class.
	gNetvars = std::make_unique<NetvarTree>(); //Initialize the netvars
}
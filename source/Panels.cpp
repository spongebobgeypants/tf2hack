#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "CNetvarManager.h"
#include "Globals.h"
#include "CMiscellaneous.h"
#include "CVisuals.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{

		if (!strcmp("HudScope", gInts.Panels->GetName(vguiPanel)) && gMenu::Visuals::RemoveScope) // Remove scope.
		{
			return;
		}

		VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.
		static unsigned int vguiMatSystemTopPanel;
			

		if (vguiMatSystemTopPanel == NULL)
		{
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'M' && szName[3] == 'S' ) //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
			{
				vguiMatSystemTopPanel = vguiPanel;
				Intro();
			}
		}

		if ( vguiMatSystemTopPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			
			if( gInts.Engine->IsDrawingLoadingImage() || !gInts.Engine->IsInGame( ) || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible( ) || ( ( GetAsyncKeyState(VK_F12) || gInts.Engine->IsTakingScreenshot( ) ) ) )
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
	catch(...)
	{
		Log::Fatal("Painttraverse failed!");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.
		gNetvars = std::make_unique<NetvarTree>(); //Initialize the netvars
		Log::Msg("Injection Successful"); //If the module got here without crashing, it is a good day.
	}
	catch(...)
	{
		Log::Fatal("Intro failed!");
	}
}
#include "CMovement.h"



void gMovement::OnCreateMove()
{
	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected())
	{
		return;
	}

	gMovement::AutoJump();
	gMovement::Autostrafer();
}

void gMovement::Autostrafer()
{
	//Stolen.
	if (!gMenu::Movement::Autostrafe) 
	{
		return;
	}

	if (!(g::g_LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (g::g_UserCmd->mousedx > 1 || g::g_UserCmd->mousedx < -1)
		{
			g::g_UserCmd->sidemove = g::g_UserCmd->mousedx > 1 ? 450 : -450;
		}
	}
	
}

void gMovement::AutoJump()
{
	if (!gMenu::Movement::Autojump)
	{
		return;
	}

	if (g::g_UserCmd->buttons & IN_JUMP)
	{
		int iFlags = g::g_LocalPlayer->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			g::g_UserCmd->buttons &= ~IN_JUMP;
	}
}


bool gMovement::IsPressingMovementKeys()
{
	// basically prevents u from jumping forward if u are standing still ( auto strafer )
	// no.


	if (!g::g_UserCmd)
		return false;

	if (g::g_UserCmd->buttons & IN_FORWARD ||
		g::g_UserCmd->buttons & IN_BACK || g::g_UserCmd->buttons & IN_LEFT || g::g_UserCmd->buttons & IN_RIGHT ||
		g::g_UserCmd->buttons & IN_MOVELEFT || g::g_UserCmd->buttons & IN_MOVERIGHT)
		return true;

	return false;
}
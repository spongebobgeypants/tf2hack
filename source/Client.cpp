#include "SDK.h"
#include "Client.h"
#include "Globals.h"
#include "CAim.h"
#include "CMovement.h"
#include "CMiscellaneous.h"

std::unique_ptr<VMTHook> clientModeHook;
using CreateMove_t = bool(__thiscall*)(PVOID, float, CUserCmd*);
//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{
	static auto oCreateMove = clientModeHook->GetOriginal<CreateMove_t>(21);
	oCreateMove(ClientMode, input_sample_frametime, pCommand); //Call the original.
	

	if (!pCommand->command_number)
		return false;

	CBaseEntity* pBaseEntity = GetBaseEntity(me); //Grab the local player's entity pointer.

	if (pBaseEntity == NULL)
		return oCreateMove;

	//defining global variables after checking for nullptr. 
	g::g_LocalPlayer = pBaseEntity;
	g::g_UserCmd = pCommand;

	gAim::LagCompensation::OnCreateMove();
	gMovement::OnCreateMove();

	return false;
}
//============================================================================================
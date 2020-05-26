#include "SDK.h"
#include "Client.h"
#include "Globals.h"
#include "CAim.h"
#include "CMovement.h"
#include "CMiscellaneous.h"

 CreateMove_t oCreateMove = nullptr;

//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{
	oCreateMove(ClientMode, input_sample_frametime, pCommand); //Call the original.
	
	if (!pCommand->command_number)
		return false;

	CBaseEntity* pBaseEntity = GetBaseEntity(me); //Grab the local player's entity pointer.

	if (pBaseEntity == NULL)
		return oCreateMove;

	g::g_LocalPlayer = pBaseEntity;
	g::g_UserCmd = pCommand;

	gAim::LagCompensation::OnCreateMove();
	gMovement::OnCreateMove();

	return false;
}
//============================================================================================
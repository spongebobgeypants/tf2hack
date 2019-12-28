#include "SDK.h"
#include "Client.h"
#include "Globals.h"
#include "CAim.h"
#include "CMovement.h"
#include "CMiscellaneous.h"

//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, int edx, float input_sample_frametime, CUserCmd* pCommand)
{

	VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	bool bReturn = hook.GetMethod<bool(__thiscall*)(PVOID, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.

	try
	{
		if (!pCommand->command_number)
			return false;

		CBaseEntity* pBaseEntity = GetBaseEntity(me); //Grab the local player's entity pointer.

		if (pBaseEntity == NULL)
			return bReturn;

		//defining global variables after checking for nullptr. 
		g::g_LocalPlayer = pBaseEntity;
		g::g_UserCmd = pCommand;
		
		gAim::LagCompensation::OnCreateMove();
		gMovement::OnCreateMove();
		
	}
	catch(...)
	{
		Log::Fatal("Createmove failed!");
	}
	return false;
}
//============================================================================================
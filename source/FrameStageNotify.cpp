#include "FrameStageNotify.h"
#include "CVisuals.h"
#include "CMiscellaneous.h"
#include "CSkinchanger.h"

void __fastcall FrameStageNotify(PVOID CHLClient, void *_this, ClientFrameStage_t Stage)
{
	gSkinchanger::Run(Stage);

	//VMTManager &HTCCNKBRKYLC = VMTManager::GetHook(CHLClient);
	//return HTCCNKBRKYLC.GetMethod<void(__fastcall *)(PVOID, void *, ClientFrameStage_t)>(35)(CHLClient, _this, Stage);
}


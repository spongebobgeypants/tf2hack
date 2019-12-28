#include "CMiscellaneous.h"

bool gMiscellaneous::ShouldDisableInterpolation;

void gMiscellaneous::BypassPure()
{
	if (!gMenu::Miscellaneous::BypassPure)
	{
		return;
	}

	//credits gir489. 
	bool check = false;
	if (!gInts.Engine->IsInGame() && check == false)
	{
		void** pure_addr = nullptr;
		if (!pure_addr)
		{
			pure_addr = *reinterpret_cast<void***>(gSignatures.GetEngineSignature("A1 ? ? ? ? 56 33 F6 85 C0") + 1);
		} *
			pure_addr = (void*)0;

		check = true;
	}
}

void OptimizeNetworkCommands()
{

	//dont @ me, please. i thought it was a good idea at 2am.

	static bool once;

	if (!once)
	{
		once = true;

		// Run this once to prevent fps loss.
		ConVar* cl_interp = gInts.Cvar->FindVar("cl_interp");
		cl_interp->SetValue("0.033");
		ConVar* cl_cmdrate = gInts.Cvar->FindVar("cl_cmdrate");
		cl_cmdrate->SetValue("67");
		ConVar* cl_interp_ratio = gInts.Cvar->FindVar("cl_interp_ratio");
		cl_interp_ratio->SetValue("1");
		ConVar* cl_lagcompensation = gInts.Cvar->FindVar("cl_lagcompensation");
		cl_lagcompensation->SetValue("1");
		ConVar* cl_pred_optimize = gInts.Cvar->FindVar("cl_pred_optimize");
		cl_pred_optimize->SetValue("2");
		ConVar* cl_smooth = gInts.Cvar->FindVar("cl_smooth");
		cl_smooth->SetValue("0");
		ConVar* cl_smoothtime = gInts.Cvar->FindVar("cl_smoothtime");
		cl_smoothtime->SetValue("0.01");
		ConVar* cl_updaterate = gInts.Cvar->FindVar("cl_updaterate");
		cl_updaterate->SetValue("67");
		ConVar* rate = gInts.Cvar->FindVar("rate");
		rate->SetValue("60000");
	}
}

void gMiscellaneous::OptimizeNetworking()
{
	gMiscellaneous::ShouldDisableInterpolation = false;

	if (gMenu::Miscellaneous::OptimizeNetworking == 0)
	{
		return; // 0 = off
	}

	if (gMenu::Miscellaneous::OptimizeNetworking == 1)
	{
		OptimizeNetworkCommands();
	}
	else if (gMenu::Miscellaneous::OptimizeNetworking == 2)
	{
		OptimizeNetworkCommands();
		gMiscellaneous::ShouldDisableInterpolation = true;
	}
}

bool gMiscellaneous::IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	//this just kills your fps.
	trace_t Trace;
	Ray_t Ray;	
	CTraceFilter Filter;

	Filter.pSkip = pLocal;

	Ray.Init(vStart, vEnd);

	gInts.EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity);
}


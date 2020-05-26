#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "EndScene.h"
#include "FrameStageNotify.h"
#include "VMT.h"

HINSTANCE HThisModule;

CPlayerVariables gPlayerVars;
CInterfaces gInts;

CreateInterface_t EngineFactory = NULL;
CreateInterface_t ClientFactory = NULL;
CreateInterface_t VGUIFactory = NULL;
CreateInterface_t VGUI2Factory = NULL;
IDirect3DDevice9* pD3DDevice = NULL;
CreateInterface_t CvarFactory = NULL;
extern EndSceneFn oEndScene;

unsigned int get_virtual(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }



DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	
	//Grab the factorys from their resptive module's EAT.
	ClientFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("client.dll"), "CreateInterface");
	EngineFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("engine.dll"), "CreateInterface");
	CvarFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vstdlib.dll"), "CreateInterface");
	VGUIFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");
	VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");

	gInts.Client = (CHLClient*)ClientFactory("VClient017", NULL);

	gInts.EntList = (CEntList*)ClientFactory("VClientEntityList003", NULL);
	
	gInts.Engine = (EngineClient*)EngineFactory("VEngineClient013", NULL);
	
	gInts.Surface = (ISurface*)VGUIFactory("VGUI_Surface030", NULL);
	
	gInts.Cvar = (ICvar*)CvarFactory("VEngineCvar004", NULL);

	gInts.EngineTrace = (IEngineTrace*)EngineFactory("EngineTraceClient003", NULL);

	gInts.ModelInfo = (IVModelInfo*)EngineFactory("VModelInfoClient006", NULL);

	gInts.Panels = (IPanel*)VGUI2Factory("VGUI_Panel009", NULL);

	gInts.DebugOverlay = (CDebugOverlay*)(EngineFactory("VDebugOverlay003", NULL));

	pD3DDevice = **reinterpret_cast<IDirect3DDevice9***>(gSignatures.FindPatternEx("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);

	hWindow = FindWindowA("Valve001", nullptr);

	pOldWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWindow, GWLP_WNDPROC, LONG_PTR(hkWndProc)));

	gInts.gGlobals = *reinterpret_cast<CGlobals **>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 8);

	Font::Get().Create(); // Initialize the font(s)

	DWORD dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 02 D9 05");

	gInts.ClientMode = **(ClientModeShared***)(dwClientModeAddress + 2);

	auto createmove = reinterpret_cast<void*>(get_virtual(gInts.ClientMode, 21));

	auto endscene = reinterpret_cast<void*>(get_virtual(pD3DDevice, 42));

	auto painttraverse = reinterpret_cast<void*>(get_virtual(gInts.Panels, 41));

	if (MH_Initialize() != MH_OK)
	{
		return FALSE;
	}

	if (MH_CreateHook(createmove, &Hooked_CreateMove, reinterpret_cast<void**>(&oCreateMove)) != MH_OK)
	{
		return FALSE;
	}

	if (MH_CreateHook(painttraverse, &Hooked_PaintTraverse, reinterpret_cast<void**>(&oPaintTraverse)) != MH_OK)
	{
		return FALSE;
	}

	if (MH_CreateHook(endscene, &hkEndScene, reinterpret_cast<void**>(&oEndScene)) != MH_OK)
	{
		return FALSE;
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return FALSE;
	}
		
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
	    CloseHandle(CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)dwMainThread, NULL, 0, NULL )); 
	}

	return true;
}
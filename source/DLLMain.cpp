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

struct HandleDisposer
{
	void operator()(const HANDLE handle) const noexcept
	{
		CloseHandle(handle);
	}
};

DWORD WINAPI dwMainThread( LPVOID lpArguments )
{
	if (gInts.Client == NULL) //Prevent repeat callings.
	{

		//Grab the factorys from their resptive module's EAT.
		ClientFactory = ( CreateInterfaceFn ) GetProcAddress(gSignatures.GetModuleHandleSafe( "client.dll" ), "CreateInterface");
		EngineFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("engine.dll"), "CreateInterface");
		CvarFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vstdlib.dll"), "CreateInterface");
		VGUIFactory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vguimatsurface.dll"), "CreateInterface");
		VGUI2Factory = (CreateInterfaceFn)GetProcAddress(gSignatures.GetModuleHandleSafe("vgui2.dll"), "CreateInterface");

		gInts.Client = ( CHLClient* )ClientFactory( "VClient017", NULL);
		gInts.EntList = (CEntList*)ClientFactory("VClientEntityList003", NULL);
		gInts.Engine = (EngineClient*)EngineFactory("VEngineClient013", NULL);
		gInts.Surface = (ISurface*)VGUIFactory("VGUI_Surface030", NULL);
		gInts.Cvar = (ICvar*)CvarFactory("VEngineCvar004", NULL);
		gInts.EngineTrace = (IEngineTrace*)EngineFactory("EngineTraceClient003", NULL);
		gInts.ModelInfo = (IVModelInfo*)EngineFactory("VModelInfoClient006", NULL);
		gInts.Panels = (IPanel*)VGUI2Factory("VGUI_Panel009", NULL);

		pD3DDevice = **reinterpret_cast<IDirect3DDevice9***>(gSignatures.FindPatternEx("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		hWindow = FindWindowA("Valve001", nullptr);
		pOldWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWindow, GWLP_WNDPROC, LONG_PTR(hkWndProc)));
		gInts.gGlobals = *reinterpret_cast<CGlobals **>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 8);

		XASSERT(gInts.Client);
		XASSERT(gInts.Panels);
		XASSERT(gInts.EntList);
		XASSERT(gInts.Engine);
		XASSERT(gInts.Surface);
		XASSERT(gInts.Cvar);
		XASSERT(gInts.EngineTrace);
		XASSERT(gInts.ModelInfo);
		XASSERT(gInts.gGlobals);


		panelHook = std::make_unique<VMTHook>(gInts.Panels);
		panelHook->Hook(41, Hooked_PaintTraverse);

		Font::Get().Create(); // Initialize the font(s)

		DWORD dwClientModeAddress = gSignatures.GetClientSignature("8B 0D ? ? ? ? 8B 02 D9 05");
		XASSERT(dwClientModeAddress);
		gInts.ClientMode = **(ClientModeShared***)(dwClientModeAddress + 2);
		LOGDEBUG("g_pClientModeShared_ptr client.dll+0x%X", (DWORD)gInts.ClientMode - dwClientBase);

		clientModeHook = std::make_unique<VMTHook>(gInts.ClientMode);
		clientModeHook->Hook(21, Hooked_CreateMove); 
		
		//Idk
		CVMTHookManager2* g_pD3DDeviceHook = NULL;
		g_pD3DDeviceHook = new CVMTHookManager2(pD3DDevice);
		oEndScene = g_pD3DDeviceHook->hook<EndSceneFn>(42, reinterpret_cast<void*>(hkEndScene));
		
	}

	while (true)
	{
		getchar(); //Keep the mainthread running.
	}

	//Laxol, if i did something mega dumb, i apologize, i am just a beginner.
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		std::unique_ptr<void, HandleDisposer>(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)dwMainThread, 0, 0, 0)); //Supposed to manually close it once it goes out of scope. Thank you laxol.
	}
	return true;
}
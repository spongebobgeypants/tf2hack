#include "CMiscellaneous.h"



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



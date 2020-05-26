#include "CSkinchanger.h"


void gSkinchanger::Run(ClientFrameStage_t stage)
{
	//credits: blackfire62, seriously man, you are the boss.

	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		return; // only do it on this stage.
	}

	if (g::g_LocalPlayer == NULL || g::g_LocalPlayer->GetLifeState() != LIFE_ALIVE)
	{
		return; // no need to run if we arent alive or are null
	}

	if (!gMenu::Miscellaneous::Australium)
	{
		return; 
	}

	
	auto list = g::g_LocalPlayer->GetActiveWeapon()->GetAttributeList();
	int* id = g::g_LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex();

	if (!list || list->m_Attributes.Count() == 0)
	{
		return;
	}

	switch (*id)
	{
	case WPN_SniperRifle:
		*id = WPN_NewSniperRifle; //All normal weapons need to be changed to their New version to apply attributes!
		list->AddAttribute(AT_is_australium_item, true);
		list->AddAttribute(AT_loot_rarity, true);
		list->AddAttribute(AT_item_style_override, true); //Australium attributes
		break;
	}
	
}
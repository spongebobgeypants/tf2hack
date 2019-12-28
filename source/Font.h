#pragma once
#include "SDK.h"
#include "Singleton.h"

#define NOMINMAX

typedef unsigned long HFont;
class Font : public Singleton<Font>
{
public:
	HFont MenuText;
	HFont ESP;
	HFont Name;
	void Create();
};
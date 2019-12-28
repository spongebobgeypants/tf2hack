#pragma once
#include "SDK.h"
#ifdef __DEBUG
#define LOGDEBUG( X, ... ) Log::Debug( X, __VA_ARGS__ )
#else
#define LOGDEBUG( X, ... )
#endif

#define XASSERT( x ) if( !x ) Log::Fatal( 0, L#x, 0, 0 );

class Log
{
public:
	static void Init(HMODULE hModule);
	static void Debug(const char* fmt, ...);
	static void Msg(const char* fmt, ...);
	static void Error(const char* fmt, ...);
	static void Fatal(const char* fmt, ...);
};
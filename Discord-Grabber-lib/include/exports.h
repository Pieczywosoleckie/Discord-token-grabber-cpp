#pragma once
#include <windows.h>

#ifdef _WIN32
	#ifdef Discord_Grabber_EXPORTS
		#define GRABBER extern "C" __declspec(dllexport)
	#else
		#define GRABBER extern "C" __declspec(dllimport)
	#endif
#else
	#define GRABBER
#endif

GRABBER const char* DiscordGrabber();

#pragma once
#ifdef VSTA_PLATFORM_WINDOWS
	#ifdef VSTA_BUILD_DLL
		#define VSTA_API __declspec(dllexport)
	#else
		#define VSTA_API __declspec(dllimport)
	#endif
#endif

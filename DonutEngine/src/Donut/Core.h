#pragma once

#ifdef DN_PLATFORM_WINDOWS
	#ifdef DN_BUILD_DLL
		#define DONUT_API __declspec(dllexport)
	#else
		#define DONUT_API __declspec(dllimport)
	#endif
#else
	#error Donut supports only Windows for now!

#endif

#define BIT(x) (1 << x)
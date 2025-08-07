#pragma once

#ifdef MIST_PLATFORM_WINDOWS
#ifdef MIST_BUILD_DLL
#define MIST_API __declspec(dllexport)
#else
#define MIST_API __declspec(dllimport)
#endif
#else
#error Only supports Windows
#endif

#define BIT(x) (1 << x)
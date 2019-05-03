#pragma once
#include "GlobalVariables.h"
#include "MgrHooks.h"
#include <iostream>

#include "MinHook.h"
#ifdef _MSC_PLATFORM_TOOLSET_v141
#pragma comment(lib, "libMinHook.x86.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(_DEBUG)
#pragma comment(lib, "libMinHook-MDd.x86-v142.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(NDEBUG)
#pragma comment(lib, "libMinHook-MD.x86-v142.lib")
#endif

PTenThousand* P10000;
PTenThousand* PlWig;
PTenThousand* PlSwordSheath;

using TPTenThousandCTor				= PTenThousand* (__cdecl*)(void* arg);
using TSamDLCCTor					= PTenThousand* (__cdecl*)(void* arg);
using TWolfDLCCTor					= PTenThousand* (__cdecl*)(void* arg);
using TWigConstructor				= PTenThousand* (__cdecl*)(void* arg);
using TSwordSheathConstructor		= PTenThousand* (__cdecl*)(void* arg);

TPTenThousandCTor		opCTor;
TSamDLCCTor				oSamDLCCtor;
TWolfDLCCTor			oWolfDLCCtor;
TWigConstructor			oWigConstructor;
TSwordSheathConstructor oSwordSheathConstructor;

// Hook the PlayerClass Constructor so we always get the newest one immediately
void* hkPTenThousandCtor(void* arg) {
	auto p10000 = opCTor(arg);
	P10000 = (PTenThousand*)p10000;
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return p10000;
}

// Hook the SamDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkSamDLCCtor(void* arg) {
	auto p10000 = oSamDLCCtor(arg);
	P10000 = (PTenThousand*)p10000;
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return p10000;
}

// Hook the WolfDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkWolfDLCCtor(void* arg) {
	auto p10000 = oWolfDLCCtor(arg);
	P10000 = (PTenThousand*)p10000;
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return p10000;
}

// Hook the Wig Constructor so we always get the newest one immediately
void* hkWigCtor(void* arg) {
	auto wig = oWigConstructor(arg);
	PlWig = (PTenThousand*)wig;
	std::cout << "New Wig at: 0x" << PlWig << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return wig;
}

// Hook the Sheath Constructor so we always get the newest one immediately
void* hkSwordSheathCtor(void* arg) {
	auto swordsheath = oSwordSheathConstructor(arg);
	PlSwordSheath = (PTenThousand*)swordsheath;
	std::cout << "New SwordSheath at: 0x" << swordsheath << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return swordsheath;
}

MgrHooks::MgrHooks()
{
	m_PTenThousandConstructorRVA	= 0x6C3340;
	m_WigConstructorRVA				= 0x6BCB20;
	m_SwordSheathConstructorRVA		= 0x6BCB70;
	m_SamDLCConstructorRVA			= 0x6C3D00;
	m_WolfDLCConstructorRVA			= 0x6C3D50;

	if (BaseAddress != NULL && MH_Initialize() == MH_ERROR_ALREADY_INITIALIZED | MH_ERROR_ALREADY_CREATED | MH_OK) {
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_PTenThousandConstructorRVA), &hkPTenThousandCtor, &opCTor) != MH_OK)
			LogToConsole("Failed to hook PTenThousand-Constructor", ELogType::Error);
		else
			LogToConsole("Hooked PTenThousand-Constructor", ELogType::Hook);

		if(MH_CreateHookEx((LPVOID)(BaseAddress + m_WigConstructorRVA), &hkWigCtor, &oWigConstructor) != MH_OK)
			LogToConsole("Failed to hook Wig-Constructor", ELogType::Error);
		else
			LogToConsole("Hooked Wig-Constructor", ELogType::Hook);

		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SwordSheathConstructorRVA), &hkSwordSheathCtor, &oSwordSheathConstructor) != MH_OK)
			LogToConsole("Failed to hook SwordSheath-Constructor", ELogType::Error);
		else
			LogToConsole("Hooked SwordSheath-Constructor", ELogType::Hook);

		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SamDLCConstructorRVA), &hkSamDLCCtor, &oSamDLCCtor) != MH_OK)
			LogToConsole("Failed to hook SamDLC-Constructor", ELogType::Error);
		else
			LogToConsole("Hooked SamDLC-Constructor", ELogType::Hook);
		
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WolfDLCConstructorRVA), &hkWolfDLCCtor, &oWolfDLCCtor) != MH_OK)
			LogToConsole("Failed to hook WolfDLC-Constructor", ELogType::Error);
		else
			LogToConsole("Hooked WolfDLC-Constructor", ELogType::Hook);

		if (MH_QueueEnableHook(MH_ALL_HOOKS) != MH_OK)
			LogToConsole("Failed to queue hooks!", ELogType::Error);
		if(MH_ApplyQueued() != MH_OK)
			LogToConsole("Failed to apply queued hooks!", ELogType::Error);
	}
}


MgrHooks::~MgrHooks()
{
	m_WigConstructorRVA = NULL;
}

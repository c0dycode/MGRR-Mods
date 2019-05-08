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

using TpTenThousandDestructor		= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TSamDLCDestructor				= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TWolfDLCDestructor			= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TWigDestructor				= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TSwordSheathDestructor		= void* (__fastcall*)(void* ecx, void* edx, void* arg);


TPTenThousandCTor		opCTor;
TpTenThousandDestructor opDestructor;

TSamDLCCTor				oSamDLCCtor;
TSamDLCDestructor		oSamDLCDestructor;

TWolfDLCCTor			oWolfDLCCtor;
TWolfDLCDestructor		oWolfDLCDestructor;

TWigConstructor			oWigConstructor;
TWigDestructor			oWigDestructor;

TSwordSheathConstructor oSwordSheathConstructor;
TSwordSheathDestructor	oSwordSheathDestructor;

// Hook the PlayerClass Constructor so we always get the newest one immediately
void* hkPTenThousandCtor(void* arg) {
	P10000 = opCTor(arg);
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return P10000;
}

// Hook PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkPTenThousandDestructor(void* ecx, void* edx, void* arg) {
	auto result = opDestructor(ecx, edx, arg);
	P10000 = NULL;
	return result;
}

// Hook the SamDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkSamDLCCtor(void* arg) {
	P10000 = oSamDLCCtor(arg);
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return P10000;
}

// Hook SamDLC-PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkSamDLCDestructor(void* ecx, void* edx, void* arg) {
	auto result = oSamDLCDestructor(ecx, edx, arg);
	P10000 = NULL;
	return result;
}

// Hook the WolfDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkWolfDLCCtor(void* arg) {
	P10000 = oWolfDLCCtor(arg);
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return P10000;
}

// Hook WolfDLC-PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkWolfDLCDestructor(void* ecx, void* edx, void* arg) {
	auto result = oWolfDLCDestructor(ecx, edx, arg);
	P10000 = NULL;
	return result;
}

// Hook the Wig Constructor so we always get the newest one immediately
void* hkWigCtor(void* arg) {
	PlWig = oWigConstructor(arg);
	std::cout << "New Wig at: 0x" << PlWig << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return PlWig;
}

// Hook Wig Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkWigDestructor(void* ecx, void* edx, void* arg) {
	auto result = oWigDestructor(ecx, edx, arg);
	PlWig = NULL;
	return result;
}

// Hook the Sheath Constructor so we always get the newest one immediately
void* hkSwordSheathCtor(void* arg) {
	PlSwordSheath = oSwordSheathConstructor(arg);
	std::cout << "New SwordSheath at: 0x" << PlSwordSheath << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return PlSwordSheath;
}

// Hook Wig Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkSwordSheathDestructor(void* ecx, void* edx, void* arg) {
	auto result = oSwordSheathDestructor(ecx, edx, arg);
	PlSwordSheath = NULL;
	return result;
}

// Class Constructor
// This is where we hook all the constructors and destructors for example
MgrHooks::MgrHooks()
{
	// PlayerClass RVA's
	m_PTenThousandConstructorRVA	= 0x6C3340;
	m_PTenThousandDestructorRVA		= 0x6C35D0;

	// Wig RVA's
	m_WigConstructorRVA				= 0x6BCB20;
	m_WigDestructorRVA				= 0x6B63E0;

	// SwordSheath RVA's
	m_SwordSheathConstructorRVA		= 0x6BCB70;
	m_SwordSheathDestructorRVA		= 0x6B6450;

	// SamDLC PlayerClass RVA's
	m_SamDLCConstructorRVA			= 0x6C3D00;
	m_SamDLCDestructorRVA			= 0x6C3C80;

	// WolfDLC PlayerClass RVA's
	m_WolfDLCConstructorRVA			= 0x6C3D50;
	m_WolfDLCDestructorRVA			= 0x6C3CA0;

	if (BaseAddress != NULL && MH_Initialize() == MH_ERROR_ALREADY_INITIALIZED | MH_ERROR_ALREADY_CREATED | MH_OK) {

		// PlayerClass Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_PTenThousandConstructorRVA), &hkPTenThousandCtor, &opCTor) != MH_OK)
			LogToConsole("Failed to hook PTenThousand-Constructor", ELogType::Error);

		// PlayerClass Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_PTenThousandDestructorRVA), &hkPTenThousandDestructor, &opDestructor) != MH_OK)
			LogToConsole("Failed to hook PTenThousand-Destructor", ELogType::Error);

		// Wig Constructor Hook
		if(MH_CreateHookEx((LPVOID)(BaseAddress + m_WigConstructorRVA), &hkWigCtor, &oWigConstructor) != MH_OK)
			LogToConsole("Failed to hook Wig-Constructor", ELogType::Error);

		// Wig Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WigDestructorRVA), &hkWigDestructor, &oWigDestructor) != MH_OK)
			LogToConsole("Failed to hook Wig-Destructor", ELogType::Error);

		// SwordSheath Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SwordSheathConstructorRVA), &hkSwordSheathCtor, &oSwordSheathConstructor) != MH_OK)
			LogToConsole("Failed to hook SwordSheath-Constructor", ELogType::Error);

		// SwordSheath Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SwordSheathDestructorRVA), &hkSwordSheathDestructor, &oSwordSheathDestructor) != MH_OK)
			LogToConsole("Failed to hook SwordSheath-Destructor", ELogType::Error);

		// SamDLC PlayerClass Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SamDLCConstructorRVA), &hkSamDLCCtor, &oSamDLCCtor) != MH_OK)
			LogToConsole("Failed to hook SamDLC-Constructor", ELogType::Error);

		// SamDLC PlayerClass Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SamDLCDestructorRVA), &hkSamDLCDestructor, &oSamDLCDestructor) != MH_OK)
			LogToConsole("Failed to hook SamDLC-Destructor", ELogType::Error);
		
		// WolfDLC PlayerClass Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WolfDLCConstructorRVA), &hkWolfDLCCtor, &oWolfDLCCtor) != MH_OK)
			LogToConsole("Failed to hook WolfDLC-Constructor", ELogType::Error);

		// WolfDLC PlayerClass Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WolfDLCDestructorRVA), &hkWolfDLCDestructor, &oWolfDLCDestructor) != MH_OK)
			LogToConsole("Failed to hook WolfDLC-Destructor", ELogType::Error);

		if (MH_QueueEnableHook(MH_ALL_HOOKS) != MH_OK)
			LogToConsole("Failed to queue hooks!", ELogType::Error);
		if (MH_ApplyQueued() != MH_OK)
			LogToConsole("Failed to apply queued hooks!", ELogType::Error);
		else
			LogToConsole("Successfully applied queued hooks!", ELogType::Hook);
	}
}

// MGR Hooks Destructor
// Unhook all the hooks from above
MgrHooks::~MgrHooks()
{
	if (MH_QueueDisableHook(MH_ALL_HOOKS) != MH_OK)
		LogToConsole("Failed to queue hooks!", ELogType::Error);
	if (MH_ApplyQueued() != MH_OK)
		LogToConsole("Failed to disable queued hooks!", ELogType::Error);
	else
		LogToConsole("Successfully unhooked constructors/destructors", ELogType::Hook);

	// PlayerClass RVA's
	m_PTenThousandConstructorRVA	= NULL;
	m_PTenThousandDestructorRVA		= NULL;

	// Wig RVA's
	m_WigConstructorRVA				= NULL;
	m_WigDestructorRVA				= NULL;

	// SwordSheath RVA's
	m_SwordSheathConstructorRVA		= NULL;
	m_SwordSheathDestructorRVA		= NULL;

	// SamDLC PlayerClass RVA's
	m_SamDLCConstructorRVA			= NULL;
	m_SamDLCDestructorRVA			= NULL;

	// WolfDLC PlayerClass RVA's
	m_WolfDLCConstructorRVA			= NULL;
	m_WolfDLCDestructorRVA			= NULL;
}

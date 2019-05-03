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
using TWigConstructor				= PTenThousand* (__cdecl*)(void* arg);
using TSwordSheathConstructor		= PTenThousand* (__cdecl*)(void* arg);

TPTenThousandCTor		opCTor;
TWigConstructor			oWigConstructor;
TSwordSheathConstructor oSwordSheathConstructor;

void* hkPTenThousandCtor(void* arg) {
	auto p10000 = opCTor(arg);
	P10000 = (PTenThousand*)p10000;	
	std::cout << "New Player at: 0x" << P10000 << "\n";
	return p10000;
}

void* hkWigCtor(void* arg) {
	auto wig = oWigConstructor(arg);
	PlWig = (PTenThousand*)wig;
	std::cout << "New Wig at: 0x" << PlWig << "\n";
	return wig;
}

void* hkSwordSheathCtor(void* arg) {
	auto swordsheath = oSwordSheathConstructor(arg);
	PlSwordSheath = (PTenThousand*)swordsheath;
	std::cout << "New SwordSheath at: 0x" << swordsheath << "\n";
	return swordsheath;
}

MgrHooks::MgrHooks()
{
	m_PTenThousandConstructorRVA	= 0x6C3340;
	m_WigConstructorRVA				= 0x6BCB20;
	m_SwordSheathConstructorRVA		= 0x6BCB70;

	opCTor = (TPTenThousandCTor)(BaseAddress + m_PTenThousandConstructorRVA);
	oWigConstructor = (TWigConstructor)(BaseAddress + m_WigConstructorRVA);
	oSwordSheathConstructor = (TSwordSheathConstructor)(BaseAddress + m_SwordSheathConstructorRVA);

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

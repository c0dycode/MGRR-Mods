#pragma once
#include "GlobalVariables.h"
#include "MgrHooks.h"
#include <iostream>
#include <sstream>

#include "MinHook.h"
#ifdef _MSC_PLATFORM_TOOLSET_v141
#pragma comment(lib, "libMinHook.x86.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(_DEBUG)
#pragma comment(lib, "libMinHook-MDd.x86-v142.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(NDEBUG)
#pragma comment(lib, "libMinHook-MD.x86-v142.lib")
#endif

PTenThousand*	P10000;
PTenThousand*	PlWig;
PTenThousand*	PlSwordSheath;
void*			CustomizeSelMenu;
void*			ObjReadSystem;

ECharacter CurrentCharacter;

using TPTenThousandCTor				= PTenThousand* (__cdecl*)(void* arg);
using TSamDLCCTor					= PTenThousand* (__cdecl*)(void* arg);
using TWolfDLCCTor					= PTenThousand* (__cdecl*)(void* arg);
using TWigConstructor				= PTenThousand* (__cdecl*)(void* arg);
using TSwordSheathConstructor		= PTenThousand* (__cdecl*)(void* arg);
using TObjReadSystemConstructor		= void* (__fastcall*)(void* arg);
using TCustomizeSelMenuConstructor	= void* (__cdecl*)(void* arg);

using TObjectIDToNameLogger			= BOOL(__cdecl*)(char* szObjectName, rsize_t size, unsigned int objectID, int param_4);

using TpTenThousandDestructor		= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TSamDLCDestructor				= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TWolfDLCDestructor			= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TWigDestructor				= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TSwordSheathDestructor		= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TObjReadSystemDestructor		= void* (__fastcall*)(void* ecx, void* edx, void* arg);
using TCustomizeSelMenuDestructor	= void* (__fastcall*)(void* ecx, void* edx, void* arg);

TPTenThousandCTor				opConstructor;
TpTenThousandDestructor			opDestructor;

TSamDLCCTor						oSamDLCConstructor;
TSamDLCDestructor				oSamDLCDestructor;

TWolfDLCCTor					oWolfDLCConstructor;
TWolfDLCDestructor				oWolfDLCDestructor;

TWigConstructor					oWigConstructor;
TWigDestructor					oWigDestructor;

TSwordSheathConstructor			oSwordSheathConstructor;
TSwordSheathDestructor			oSwordSheathDestructor;

TCustomizeSelMenuConstructor	oCustomizeSelmenuConstructor;
TCustomizeSelMenuDestructor		oCustomizeSelmenuDestructor;

TObjReadSystemConstructor oObjReadSystemConstructor;
TObjReadSystemDestructor oObjReadSystemDestructor;

TObjectIDToNameLogger oObjectIDToNameLogger;

// Hook the PlayerClass Constructor so we always get the newest one immediately
void* hkPTenThousandCtor(void* arg) {
	auto res = opConstructor(arg);
	P10000 = res;
	CurrentCharacter = ECharacter::Raiden;
	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return res;
}

// Hook PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkPTenThousandDestructor(void* ecx, void* edx, void* arg) {
	auto result = opDestructor(ecx, edx, arg);
	P10000 = NULL;
	CurrentCharacter = ECharacter::None;
	return result;
}

// Hook the SamDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkSamDLCCtor(void* arg) {
	P10000 = oSamDLCConstructor(arg);
	std::cout << "New Player at: 0x" << P10000 << "\n";

	CurrentCharacter = ECharacter::Sam;

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return P10000;
}

// Hook SamDLC-PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkSamDLCDestructor(void* ecx, void* edx, void* arg) {
	auto result = oSamDLCDestructor(ecx, edx, arg);
	P10000 = NULL;
	CurrentCharacter = ECharacter::None;
	return result;
}

// Hook the WolfDLC-PlayerClass Constructor so we always get the newest one immediately
void* hkWolfDLCCtor(void* arg) {
	P10000 = oWolfDLCConstructor(arg);

	CurrentCharacter = ECharacter::Wolf;

	std::cout << "New Player at: 0x" << P10000 << "\n";

	// Notify the mainloop that colors will need to be applied again, since the game created a new object
	bColorsNeedRefresh = true;
	return P10000;
}

// Hook WolfDLC-PlayerClass Destructor so we can set it to NULL to avoid AccessViolations
void* __fastcall hkWolfDLCDestructor(void* ecx, void* edx, void* arg) {
	auto result = oWolfDLCDestructor(ecx, edx, arg);
	P10000 = NULL;
	CurrentCharacter = ECharacter::None;
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


// Hook the CustomizeSelMenu Constructor so we always get the newest one immediately
void* hkCustomizeSelmenuCtor(void* arg) {
	if (!CustomizeSelMenu) {
		CustomizeSelMenu = oCustomizeSelmenuConstructor(arg);
		std::cout << "New CustomizeSelMenu at: 0x" << CustomizeSelMenu << "\n";
	}	
	return CustomizeSelMenu;
}

// Hook CustomizeSelMenu Destructor
void* __fastcall hkCustomizeSelmenuDestructor(void* ecx, void* edx, void* arg) {
	// auto result = oCustomizeSelmenuDestructor(ecx, edx, arg);

	// CustomizeSelMenu = NULL;

	// Possibly prevent the game from deleting this
	return NULL;
}

// Hook the ObjReadSystemMenu Constructor so we always get the newest one immediately
void* __fastcall hkObjReadSystemCtor(void* arg) {
	ObjReadSystem = oObjReadSystemConstructor(arg);
	std::cout << "New ObjReadSystem at: 0x" << ObjReadSystem << "\n";
	return ObjReadSystem;
}

// Hook ObjReadSystem Destructor
void* __fastcall hkObjReadSystemDestructor(void* ecx, void* edx, void* arg) {
	auto result = oObjReadSystemDestructor(ecx, edx, arg);

	ObjReadSystem = NULL;
	return result;
}

void SimpleLog(float x, float y, float z) {
	std::stringstream sstream;

	sstream << "X: " << x << "\n";
	OutputDebugStringA(sstream.str().c_str());
	sstream << "Y: " << y << "\n";
	OutputDebugStringA(sstream.str().c_str());
	sstream << "Z: " << z << "\n";
	OutputDebugStringA(sstream.str().c_str());
}

// Hook ObjectIDToName
// Info found here: https://www.unknowncheats.me/forum/other-games/312198-nier-automata-info.html
// Nier also uses the Platinum Engine, so stuff should be pretty similar
BOOL __cdecl hkObjIDToName(char* szObjectName, rsize_t size, unsigned int objectID, int param_4) {
	auto result = oObjectIDToNameLogger(szObjectName, size, objectID, param_4);
	std::cout << "ID " << objectID << " : " << szObjectName << "\n";
	return result;
}

static float x = 0.0f;
static float y = 0.0f;
static float z = 0.0f;
DWORD _eax;
DWORD jumpBack;
// We're hooking in the middle of a function, so we need the address we need to jump back to
void __declspec(naked) hkMovementModifier() {
	
	__asm push eax

	if (P10000) {
		__asm mov _eax, eax
		// Check if the function works on the PlayerCoordinates atm
		if (_eax == (DWORD)P10000 && (
			/* Slow Walk 
			P10000->CurrentInput == 2 ||*/  
			/* Running */ 
			P10000->CurrentInput == 3 && CurrentCharacter == ECharacter::Raiden || 
			P10000->CurrentInput == 2 && CurrentCharacter == ECharacter::Sam ||
			P10000->CurrentInput == 2 && CurrentCharacter == ECharacter::Wolf ||
			/* Sprinting */ 
			P10000->CurrentInput == 71 && CurrentCharacter == ECharacter::Raiden ||
			P10000->CurrentInput == 8 && CurrentCharacter == ECharacter::Sam ||
			P10000->CurrentInput == 4 && CurrentCharacter == ECharacter::Wolf)) {
			__asm {

				//mov eax, _eax
				mov eax, [esp + 0x14]
				mov x, eax
				mov eax, [esp + 0x18]
				mov y, eax
				mov eax, [esp+0x1C]
				mov z, eax
				/*mov eax, _eax
				mov eax, [eax + 0x58]
				mov z, eax*/
			}
			//SimpleLog(x, y, z);
			x *= movementSpeedMultiplier;
			y *= movementSpeedMultiplier;
			z *= movementSpeedMultiplier;

			__asm {
				mov eax, x
				mov [esp + 0x14], eax

				mov eax, y
				mov [esp + 0x18], eax

				mov eax, z
				mov [esp + 0x1C], eax

				/*push ebx
				mov ebx, z
				mov eax, _eax
				mov[eax + 0x54], ebx
				pop ebx*/
			}
		}
	}
	__asm {
		pop eax

		// Need to include these from the original code, since we do not hook something that's 5 bytes long
		fld dword ptr [eax + 0x50]
		mov ecx, [esp + 0x000000BC]

		// Jump back to where the could should continue
		jmp jumpBack
	}
}

// Class Constructor
// This is where we hook all the constructors and destructors for example
MgrHooks::MgrHooks()
{
	// PlayerClass RVA's
	m_PTenThousandConstructorRVA		= 0x6C3340;
	m_PTenThousandDestructorRVA			= 0x6C35D0;

	// Wig RVA's
	m_WigConstructorRVA					= 0x6BCB20;
	m_WigDestructorRVA					= 0x6B63E0;

	// SwordSheath RVA's
	m_SwordSheathConstructorRVA			= 0x6BCB70;
	m_SwordSheathDestructorRVA			= 0x6B6450;

	// SamDLC PlayerClass RVA's
	m_SamDLCConstructorRVA				= 0x6C3D00;
	m_SamDLCDestructorRVA				= 0x6C3C80;

	// WolfDLC PlayerClass RVA's
	m_WolfDLCConstructorRVA				= 0x6C3D50;
	m_WolfDLCDestructorRVA				= 0x6C3CA0;

	// CustomizeSelMenu RVA's
	m_CustomizeSelMenuConstructorRVA	= 0x5A09E0;
	m_CustomizeSelMenuDestructorRVA		= 0x5B2B00;

	// MovementSpeed Modifier RVA
	m_MovementSpeedModifierRVA			= 0xA303FC;

	// ObjReadSystem RVA's
	m_ObjReadSystemConstructorRVA		= 0x00a9f5e0;
	m_ObjReadSystemDestructorRVA		= 0x00a9fac0;

	if (BaseAddress != NULL && MH_Initialize() == MH_ERROR_ALREADY_INITIALIZED | MH_ERROR_ALREADY_CREATED | MH_OK) {

		// PlayerClass Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_PTenThousandConstructorRVA), &hkPTenThousandCtor, &opConstructor) != MH_OK)
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
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SamDLCConstructorRVA), &hkSamDLCCtor, &oSamDLCConstructor) != MH_OK)
			LogToConsole("Failed to hook SamDLC-Constructor", ELogType::Error);

		// SamDLC PlayerClass Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_SamDLCDestructorRVA), &hkSamDLCDestructor, &oSamDLCDestructor) != MH_OK)
			LogToConsole("Failed to hook SamDLC-Destructor", ELogType::Error);
		
		// WolfDLC PlayerClass Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WolfDLCConstructorRVA), &hkWolfDLCCtor, &oWolfDLCConstructor) != MH_OK)
			LogToConsole("Failed to hook WolfDLC-Constructor", ELogType::Error);

		// WolfDLC PlayerClass Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_WolfDLCDestructorRVA), &hkWolfDLCDestructor, &oWolfDLCDestructor) != MH_OK)
			LogToConsole("Failed to hook WolfDLC-Destructor", ELogType::Error);

		// ObjReadSystem Constructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_ObjReadSystemConstructorRVA), &hkObjReadSystemCtor, &oObjReadSystemConstructor) != MH_OK)
			LogToConsole("Failed to hook ObjReadSystem-Constructor", ELogType::Error);

		// ObjReadSystem Destructor Hook
		if (MH_CreateHookEx((LPVOID)(BaseAddress + m_ObjReadSystemDestructorRVA), &hkObjReadSystemDestructor, &oObjReadSystemDestructor) != MH_OK)
			LogToConsole("Failed to hook ObjReadSystem-Destructor", ELogType::Error);

		//// CustomizeSelMenu Constructor Hook
		//if (MH_CreateHookEx((LPVOID)(BaseAddress + m_CustomizeSelMenuConstructorRVA), &hkCustomizeSelmenuCtor, &oCustomizeSelmenuConstructor) != MH_OK)
		//	LogToConsole("Failed to hook CustomizeSelMenu-Constructor", ELogType::Error);

		//// CustomizeSelMenu Destructor Hook
		//if (MH_CreateHookEx((LPVOID)(BaseAddress + m_CustomizeSelMenuDestructorRVA), &hkCustomizeSelmenuDestructor, &oCustomizeSelmenuDestructor) != MH_OK)
		//	LogToConsole("Failed to hook CustomizeSelMenu-Destructor", ELogType::Error);

#ifdef _DEBUG
		if (MH_CreateHookEx((LPVOID)(BaseAddress + 0x005f8ea0), &hkObjIDToName, &oObjectIDToNameLogger) != MH_OK)
			LogToConsole("Failed to hook ObjIDToName", ELogType::Error);
#endif

		jumpBack = (BaseAddress + m_MovementSpeedModifierRVA + 10);
		// MovementSpeed Modifier Hook 
		if (MH_CreateHook((LPVOID)(BaseAddress + m_MovementSpeedModifierRVA), &hkMovementModifier, NULL) != MH_OK)
			LogToConsole("Failed to hook MovementSpeed Modifier", ELogType::Error);

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
	m_PTenThousandConstructorRVA		= NULL;
	m_PTenThousandDestructorRVA			= NULL;

	// Wig RVA's
	m_WigConstructorRVA					= NULL;
	m_WigDestructorRVA					= NULL;

	// SwordSheath RVA's
	m_SwordSheathConstructorRVA			= NULL;
	m_SwordSheathDestructorRVA			= NULL;

	// SamDLC PlayerClass RVA's
	m_SamDLCConstructorRVA				= NULL;
	m_SamDLCDestructorRVA				= NULL;

	// WolfDLC PlayerClass RVA's
	m_WolfDLCConstructorRVA				= NULL;
	m_WolfDLCDestructorRVA				= NULL;

	// CustomizeSelMenu RVA's
	m_CustomizeSelMenuConstructorRVA	= NULL;
	m_CustomizeSelMenuDestructorRVA		= NULL;

	m_MovementSpeedModifierRVA			= NULL;
}

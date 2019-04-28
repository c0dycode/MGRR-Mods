#pragma once
#include "DetourHooking.h"

DetourHooking::DetourHooking()
{
}


DetourHooking::~DetourHooking()
{
}

bool DetourHooking::Hook(void* targetAddress, void* detourCode, size_t amountOfOPCodes)
{
	// Store old protection in here
	DWORD oldProtect;

	// Change Protection of the Target so we can modify it;
	VirtualProtect(targetAddress, amountOfOPCodes, PAGE_EXECUTE_READWRITE, &oldProtect);
	std::vector<char*> opCodes;
	memcpy(opCodes.data(), targetAddress, amountOfOPCodes);
	m_installedHooks.insert(std::make_pair(targetAddress, *opCodes.data()));

	// TODO: Properly implement jmp and return
	//memcpy(targetAddress, detourCode, amountOfOPCodes);
	// Restore old protection
	VirtualProtect(targetAddress, amountOfOPCodes, PAGE_EXECUTE_READWRITE, &oldProtect);

	return true;
}

bool DetourHooking::Restore(void* targetAddress)
{
	return false;
}

#pragma once
#include <Windows.h>
#include <map>
#include <vector>

class DetourHooking
{
public:
	DetourHooking();
	~DetourHooking();
	
	bool Hook(void* targetAddress, void* detourCode, size_t amountOfOPCodes);
	bool Restore(void* targetAddress);

private:
	std::map<void*, char*> m_installedHooks;
};


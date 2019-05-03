#pragma once
#include <Windows.h>

class MgrHooks
{
public:
	MgrHooks();
	~MgrHooks();

private:
	uintptr_t m_PTenThousandConstructorRVA;
	uintptr_t m_WigConstructorRVA;
	uintptr_t m_SwordSheathConstructorRVA;
};

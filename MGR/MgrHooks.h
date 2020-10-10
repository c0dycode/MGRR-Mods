#pragma once
#include <Windows.h>

BOOL __cdecl hkObjIDToName(char* szObjectName, rsize_t size, unsigned int objectID, int param_4);

class MgrHooks
{
public:
	MgrHooks();
	~MgrHooks();

private:
	uintptr_t m_PTenThousandConstructorRVA;
	uintptr_t m_PTenThousandDestructorRVA;

	uintptr_t m_WigConstructorRVA;
	uintptr_t m_WigDestructorRVA;

	uintptr_t m_SwordSheathConstructorRVA;
	uintptr_t m_SwordSheathDestructorRVA;

	uintptr_t m_SamDLCConstructorRVA;
	uintptr_t m_SamDLCDestructorRVA;

	uintptr_t m_WolfDLCConstructorRVA;
	uintptr_t m_WolfDLCDestructorRVA;

	uintptr_t m_CustomizeSelMenuConstructorRVA;
	uintptr_t m_CustomizeSelMenuDestructorRVA;

	uintptr_t m_ObjReadSystemConstructorRVA;
	uintptr_t m_ObjReadSystemDestructorRVA;

	uintptr_t m_MovementSpeedModifierRVA;

	uintptr_t m_PTenThousandTakeDamageRVA;
};

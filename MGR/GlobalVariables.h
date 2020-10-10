#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "MGR.h"

//#define SOL_USING_CXX_LUAJIT 1
//#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
//#include "sol.hpp"

extern uintptr_t		BaseAddress;
extern PTenThousand*	P10000;
extern PTenThousand*	LastTargetedEnemy;
extern PTenThousand*	PlWig;
extern PTenThousand*	PlSwordSheath;
extern void*			CustomizeSelMenu;
extern void*			ObjReadSystem;

//inline sol::state lua;

extern PlayerManager* PManager;
extern VRGBA* vRaidenColors;
extern ECharacter CurrentCharacter;

extern bool bShowOverlay;
extern bool bDisableTakingDamage;
extern bool bHaveOriginalVMT;
extern bool bColorsNeedRefresh;

//extern bool bIncludePlayerDamageTakenModifier;
extern float fDamageTakenMultiplier;

extern int currency;
extern int movementSpeedMultiplier;
//extern int iColorsChoice;

extern float fCameraHeigthOffset;

extern int discoDelay;
extern bool* gameDrawsCursor;

// Different types of Log-Messages
enum ELogType {
	Hook,
	Info,
	Error
};

// Simple console logging, default messagetype is "INFO"
extern void LogToConsole(const char* textToLog, ELogType logType);
#endif
#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#include "MGR.h"

extern uintptr_t BaseAddress;
extern PTenThousand* P10000;
extern PTenThousand* LastTargetedEnemy;
extern PTenThousand* PlWig;
extern PTenThousand* PlSwordSheath;

extern PlayerManager* PManager;
extern VRGBA* vRaidenColors;

extern bool bShowOverlay;
extern bool bDisableTakingDamage;
extern bool bHaveOriginalVMT;
extern int currency;
//extern int iColorsChoice;

extern float fCameraHeigthOffset;

extern int discoDelay;
extern bool* gameDrawsCursor;

// Different types of Log-Messages
extern enum ELogType {
	Hook,
	Info,
	Error
};

// Simple console logging, default messagetype is "INFO"
extern void LogToConsole(const char* textToLog, ELogType logType);
#endif
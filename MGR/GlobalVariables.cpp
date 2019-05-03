#include "GlobalVariables.h"
#include <iostream>

// Different types of Log-Messages
enum ELogType LogType;

// Simple console logging, default messagetype is "INFO"
void LogToConsole(const char* textToLog, ELogType logType = ELogType::Info) {
	switch (logType) {
	case Hook:
		std::cout << "[HOOK]: " << textToLog << "\n";
		break;
	case Info:
		std::cout << "[INFO]: " << textToLog << "\n";
		break;
	case Error:
		std::cout << "[ERROR]: " << textToLog << "\n";
		break;
	}
}
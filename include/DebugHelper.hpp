#include <istream>
#include <iostream>

namespace DebugHelper{

    void CreateConsole(){
        bool result = AllocConsole();
		FILE* con;
		if (result) {
			freopen_s(&con, "CONIN$", "r", stdin);
			freopen_s(&con, "CONOUT$", "w", stdout);
			freopen_s(&con, "CONOUT$", "w", stderr);
			std::cout << "Created console!\n";
		}
		else {
			AttachConsole(ATTACH_PARENT_PROCESS);
			freopen_s(&con, "CONIN$", "r", stdin);
			freopen_s(&con, "CONOUT$", "w", stdout);
			freopen_s(&con, "CONOUT$", "w", stderr);
			std::cout << "Attached to console!\n";
		}
    }
}
// dllmain.cpp : Defines the entry point for the DLL application.
#pragma pack(0x4)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <iostream>
#include <atomic>

#include "DebugHelper.hpp"
#include "myDXHelpers.h"
#include "VMTHooking.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "MinHook.h"
#ifdef _MSC_PLATFORM_TOOLSET_v141
#pragma comment(lib, "libMinHook.x86.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(_DEBUG)
#pragma comment(lib, "libMinHook-MDd.x86-v142.lib")
#elif defined(_MSC_PLATFORM_TOOLSET_v142) && defined(NDEBUG)
#pragma comment(lib, "libMinHook-MD.x86-v142.lib")
#endif


#include "GlobalVariables.h"
#include "Overlay.h"

uintptr_t BaseAddress;
PTenThousand* P10000;
PTenThousand* LastTargetedEnemy;

PlayerManager* PManager;
VRGBA* vRaidenColors = new VRGBA;

CRITICAL_SECTION critsecHook;
CRITICAL_SECTION critsec;
static bool init = false;

bool bShowOverlay = false;
bool bDisableTakingDamage = false;
bool bHaveOriginalVMT = false;
int currency = 0;
//int iColorsChoice = 0;

float fCameraHeigthOffset = 1.4f;

int discoDelay = 50;
bool* gameDrawsCursor = NULL;

WNDPROC oWndProc;
HWND wnd;

static VMTHooking PTenThousandOneVT;
static VMTHooking DInputVT;
static uintptr_t* PTenThousandVMT;
static uintptr_t* DInputVMT;

typedef PTenThousand* (__stdcall* TPTenThousandCTor)(void* targetMemory);
TPTenThousandCTor opCTor;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef void(__fastcall* tTakeDamage)(void* thiss, void* edx, uint32_t damageToTake, uint32_t unknown1);
tTakeDamage oTakeDamage;

void __fastcall myTakeDamage (void* thiss, void* edx, uint32_t damageToTake, uint32_t unknown1) {
	damageToTake = 0;
	return oTakeDamage(thiss, edx, damageToTake, unknown1);
}

typedef int(__cdecl* tWriteBodyPartData)(void* unknown, void* cPart, void* Animation, float someFloat, void* more);
tWriteBodyPartData oWriteBodyPartData;

// No longer needed, since I figured out how to "Freeze" the cPart change
int __cdecl hkWriteBodyPartData(void* unknown, void* ccPart, void* animation, float someFloat, void* more) {

	return oWriteBodyPartData(unknown, ccPart, animation, someFloat, more);
}

LONG OldMouseX = NULL;
LONG OldMouseY = NULL;

HHOOK MouseHook;
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (bShowOverlay) // bool
	{
		ImGuiIO& io = ImGui::GetIO();
		MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;

		if (pMouseStruct != NULL) {
			switch (wParam)
			{
			case WM_LBUTTONDOWN:
				io.MouseDown[0] = true;
				break;
			case WM_LBUTTONUP:
				io.MouseDown[0] = false;
				io.MouseReleased[0] = true;
				break;
			case WM_RBUTTONDOWN:
				io.MouseDown[1] = true;
				break;
			case WM_RBUTTONUP:
				io.MouseDown[1] = false;
				io.MouseReleased[1] = true;
				break;
			case WM_MOUSEMOVE:
				OldMouseX = pMouseStruct->pt.x;
				OldMouseY = pMouseStruct->pt.y;
			}
		}
	}
	return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}

typedef BOOL (WINAPI * TClipCursor)(RECT* lpRect);
TClipCursor oClipCursor;

BOOL WINAPI hkClipCursor(RECT* lpRect) {
	if (bShowOverlay) {
		CURSORINFO ci = { 0 };
		ci.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&ci);

		if (ci.flags == CURSOR_SHOWING)
			return oClipCursor(NULL);
		else if (GetForegroundWindow() == wnd) {
			RECT rc = { 0 };
			GetClientRect(wnd, &rc);
			return oClipCursor(&rc);
		}
	}
	return oClipCursor(lpRect);
}

static IDirect3DDevice9* d3ddevice;
static D3DPRESENT_PARAMETERS d3dpparams;

static bool bIsCursorCameraPatched = false;
static const char* eaxBackup[5] = { 0 };
static const char* ecxBackup[6] = { 0 };


// WndProc hook to control showing/hiding of the Overlay
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {		
	if (bShowOverlay) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	}

	if (GetAsyncKeyState(VK_PAUSE) & 1)
	{
		while (GetAsyncKeyState(VK_PAUSE) & 1)
			Sleep(10);

		bShowOverlay ^= 1;
		if (bShowOverlay && !bIsCursorCameraPatched) {
			DWORD oldProtect;

			// Disable the game writing the cursor position, so we can move the mouse around in-game without turning the camera
			VirtualProtect((LPVOID)(BaseAddress + 0x9F848D), 11, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy(eaxBackup, (LPVOID)(BaseAddress + 0x9F848D), 5);
			memset((LPVOID)(BaseAddress + 0x9F848D), 0x90, 5);

			memcpy(ecxBackup, (LPVOID)(BaseAddress + 0x9F8492), 6);
			memset((LPVOID)(BaseAddress + 0x9F8492), 0x90, 6);
			VirtualProtect((LPVOID)(BaseAddress + 0x9F8492), 11, oldProtect, &oldProtect);

			bIsCursorCameraPatched = true;
		}
		if (!bShowOverlay && bIsCursorCameraPatched && eaxBackup != NULL && ecxBackup != NULL) {
			DWORD oldProtect;

			// Re-Enable the game writing the cursor position, so we can move the mouse around in-game without turning the camera
			VirtualProtect((LPVOID)(BaseAddress + 0x9F848D), 11, PAGE_EXECUTE_READWRITE, &oldProtect);
			memcpy((LPVOID)(BaseAddress + 0x9F848D), eaxBackup, 5);

			memcpy((LPVOID)(BaseAddress + 0x9F8492), ecxBackup, 6);
			VirtualProtect((LPVOID)(BaseAddress + 0x9F8492), 11, oldProtect, &oldProtect);

			bIsCursorCameraPatched = false;
		}

		// Refresh Presets on Overlay-Toggle
		Configs::GetAvailablePresets();
	}

	if (bShowOverlay) {
		return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

typedef BOOL(WINAPI* SetCursorPosition)(int X, int Y);
SetCursorPosition oSetCursorPosition = NULL;

BOOL WINAPI hkSetCursorPos(int X, int Y) {
	if (bShowOverlay) {
		return oSetCursorPosition(OldMouseX, OldMouseY);
	}
	return oSetCursorPosition(X, Y);
}

typedef bool (APIENTRY* GetCursorPosition)(LPPOINT lpPoint);
GetCursorPosition oGetCursorPosition = NULL;

bool APIENTRY hkGetCursorPos(LPPOINT lpPoint)
{
	bool ret = oGetCursorPosition(lpPoint);
	if (bShowOverlay)
	{
		//ScreenToClient(GetForegroundWindow(), lpPoint);
		/*OldMouseX = lpPoint->x;
		OldMouseY = lpPoint->y;*/
		/*ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = OldMouseX;
		io.MousePos.y = OldMouseY;
		io.WantSetMousePos = true;*/
		/*lpPoint->x = OldMouseX;
		lpPoint->y = OldMouseY;*/
	}
	return ret;
}

HRESULT WINAPI Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	
	d3ddevice = pDevice;
	d3dpparams = *pPresentationParameters;
	
	/*ImGui::DestroyContext();
	ImGui::CreateContext();*/
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto hr = oResetFunction(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}



// This is where the overlay is drawn
HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	EnterCriticalSection(&critsecHook);
	// Do directx things here..
	/*D3DRECT BarRect = { 100, 100, 200, 200 };
	pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(1, 1, 1, 1), 0.0f, 0);*/

	if (!init) {
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags &= ImGuiConfigFlags_NavEnableSetMousePos;
		ImGui_ImplWin32_Init(wnd);
		ImGui_ImplDX9_Init(pDevice);
		MouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, 0, GetCurrentThreadId());
		init = true;
	}
	ImGuiIO& io = ImGui::GetIO();
	if (bShowOverlay) {
		PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);

		if (*gameDrawsCursor == false) {
			io.MouseDrawCursor = true;
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Create & handle all the Overlay controls
		Overlay::OverlayLogic();

		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	else if (!bShowOverlay && io.MouseDrawCursor == true) {
		io.MouseDrawCursor = false;
	}

	//std::cout << "[+] Execution of hooked function successfull.\n";
	LeaveCriticalSection(&critsecHook);
	return oEndScene(pDevice);
}

typedef HRESULT(__fastcall* TGetDeviceState)(void* ecx, void* edx, void* unknown, DWORD cbData, LPVOID lpvData);
TGetDeviceState oGetDeviceState;

typedef struct DIMOUSESTATE {
	LONG lX;
	LONG lY;
	LONG lZ;
	BYTE rgbButtons[4];
} DIMOUSESTATE, * LPDIMOUSESTATE;


// Hook for the DirectInput8 function to get mouse-data
HRESULT __fastcall hkGetDeviceState(void* ecx, void* edx, void* unknown, DWORD cbData, LPVOID lpvData) {
	
	auto result = oGetDeviceState(ecx, edx, unknown, cbData, lpvData);
	ImGuiIO& io = ImGui::GetIO();

	// If the Overlay is opened and mouse is on top of an overlay-window
	if (bShowOverlay && io.WantCaptureMouse) {
		auto mousestate = (DIMOUSESTATE*)lpvData;
		// If Left mousebutton is pressed, change it to be unpressed
		if (mousestate->rgbButtons[0] &= 0x80)
			mousestate->rgbButtons[0] = 0;
	}
	return result;
}

// Hooke the DirectInput-Device
void HookDInput() {
	// Make sure the device has been created after launch
	while (**(uintptr_t***)(BaseAddress + 0x9d975A) == nullptr)
		Sleep(10);
	DInputVMT = DInputVT.GetReplacedVTablePointer(**(uintptr_t***)(BaseAddress + 0x9D975A));
	oGetDeviceState = (TGetDeviceState)DInputVT.HookVMT(9, (uintptr_t)hkGetDeviceState);	
}

int exceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
	if (code == EXCEPTION_ACCESS_VIOLATION)
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	};
}

DWORD WINAPI MainThread(LPVOID param)
{
	// Create a Console window // use std::cout << "bla\n" to write to it
	DebugHelper::CreateConsole();

	while (!MyD3D9::GetD3D9Device())
	{
		Sleep(200);
	}
	BaseAddress = (uintptr_t)GetModuleHandle(NULL);

	//// Read saved Randomizer Presets
	Configs::GetAvailablePresets();

	// Should probably check this properly whether or not it succeeds
	MH_Initialize();
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::EndSceneIndex], &Hooked_EndScene, &oEndScene);
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::ResetIndex], &Hooked_Reset, &oResetFunction);
	MH_CreateHookEx((LPVOID)(BaseAddress + 0xA39380), &hkWriteBodyPartData, &oWriteBodyPartData);

	//TODO: These kinda work, but not at all at the same time :D
	MH_CreateHookEx(GetCursorPos, &hkGetCursorPos, &oGetCursorPosition);
	MH_CreateHookEx(SetCursorPos, &hkSetCursorPos, &oSetCursorPosition);
	
	MH_CreateHookEx(ClipCursor, &hkClipCursor, &oClipCursor);
	MH_QueueEnableHook(MH_ALL_HOOKS);

	while (wnd == NULL)
		wnd = **(HWND**)(BaseAddress + 0x9F8DF5);
	oWndProc = (WNDPROC)SetWindowLongPtr(wnd, GWL_WNDPROC, (LONG_PTR)WndProc);
	InitializeCriticalSection(&critsecHook);
	MH_ApplyQueued();

	// Only need to read this once	fCameraHeigthOffset = **(float**)(BaseAddress + 0x6C0C7E + 2);
	fCameraHeigthOffset = **(float**)(BaseAddress + 0x6C0C7E + 2);

	// This is the address where the game stores the value for whether to show the mouse cursor or not
	gameDrawsCursor = *(bool**)(BaseAddress + 0x9F8CF6);

	// Clear the console output
	system("cls");
	
	// Trying to hook DInput in order to ignore input to the game, while the Overlay is open
	HookDInput();

	PManager = (PlayerManager*)* (uintptr_t*)(BaseAddress + 0x17EA100);

	InitializeCriticalSection(&critsec);
	/*opCTor = (TPTenThousandCTor)(BaseAddress + 0x6C3340);
	PTenThousand* test = { 0 };*/

	while (true)
	{
		if (TryEnterCriticalSection(&critsec))
		{
			__try {
				if (P10000 && P10000->ArmorMeshes)
				{
					if (Overlay::iColorsChoice == 1) {
						if (P10000->ArmorMeshes->ArmorGlowDiscoR) {
							P10000->ArmorMeshes->ArmorGlowDiscoR = vRaidenColors->R;
							Sleep(discoDelay);
						}
						if (P10000->ArmorMeshes->ArmorGlowDiscoG) {
							P10000->ArmorMeshes->ArmorGlowDiscoG = vRaidenColors->G;
							Sleep(discoDelay);
						}
						if (P10000->ArmorMeshes->ArmorGlowDiscoB) {
							P10000->ArmorMeshes->ArmorGlowDiscoB = vRaidenColors->B;
							Sleep(discoDelay);
						}
						/*if (P10000->ArmorMeshes->ArmorGlowDiscoAlpha) {
							P10000->ArmorMeshes->ArmorGlowDiscoAlpha = vRaidenColors->A;
							Sleep(discoDelay);
						}*/
					}
					if (Overlay::iColorsChoice == 2) {
						if (P10000->ArmorMeshes->ArmorGlowR) {
							P10000->ArmorMeshes->ArmorGlowR = vRaidenColors->R;
						}
						if (P10000->ArmorMeshes->ArmorGlowG) {
							P10000->ArmorMeshes->ArmorGlowG = vRaidenColors->G;
						}
						if (P10000->ArmorMeshes->ArmorGlowB) {
							P10000->ArmorMeshes->ArmorGlowB = vRaidenColors->B;
						}
						/*if (P10000->ArmorMeshes->ArmorGlowAlpha) {
							P10000->ArmorMeshes->ArmorGlowAlpha = vRaidenColors->A;
						}*/
					}
				}
				if (!PManager)
					PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);
				else {
					currency = PManager->Cash;
				}
			}
			__except(exceptionFilter(GetExceptionCode(), GetExceptionInformation())){}
			LeaveCriticalSection(&critsec);
		}
		P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
		LastTargetedEnemy = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x17EA8C8);
		Sleep(10);
	}
	delete vRaidenColors;
	// All of the unhooking stuff; Probably need to fix the order
	(WNDPROC)SetWindowLongPtr(wnd, GWL_WNDPROC, (LONG_PTR)oWndProc);
	UnhookWindowsHookEx(MouseHook);
	MH_QueueDisableHook(MH_ALL_HOOKS);
	MH_ApplyQueued();
	MH_Uninitialize();
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	std::cout << "End!\n";
	FreeConsole();
	FreeLibraryAndExitThread((HMODULE)param, 1);
	return 1;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, nullptr);
		DisableThreadLibraryCalls(hModule);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


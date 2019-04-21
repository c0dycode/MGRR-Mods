// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <atomic>

#include "DebugHelper.hpp"
#include "myDXHelpers.h"
#include "VMTHooking.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "MinHook.h"
#pragma comment(lib, "libMinHook.x86.lib")

#include "MGR.h"

uintptr_t BaseAddress;
PTenThousand* P10000;
PlayerManager* PManager;

CRITICAL_SECTION critsecHook;
CRITICAL_SECTION critsec;
static bool init = false;
bool bShowOverlay = false;
static bool bEnableDisco = false;
static bool bDisableTakingDamage = false;
static bool bHaveOriginalVMT = false;
int partytimer = 3000;
int currency = 0;

int discoDelay = 50;
bool* gameDrawsCursor = NULL;

WNDPROC oWndProc;
HWND wnd;

static VMTHooking PTenThousandOneVT;
static uintptr_t* PTenThousandVMT;

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



HHOOK hMouseHook;
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
			}
		}
	}
	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

// WndProc hook to control showing/hiding of the Overlay
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (GetAsyncKeyState(VK_PAUSE) & 1)
	{
		while (GetAsyncKeyState(VK_PAUSE) & 1)
			Sleep(10);

		bShowOverlay ^= 1;
	}

	if (bShowOverlay && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {	
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	ImGui::DestroyContext();
	ImGui::CreateContext();
	return oResetFunction(pDevice, pPresentationParameters);
}

// Moved all the Overlay-controls related code in here
void OverlayLogic() {
	ImGui::Begin("MGR Things - by c0dycode :)");// , 0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Activate Disco Raiden", &bEnableDisco);
	if (bEnableDisco) {
		ImGui::SliderInt("Delay", &discoDelay, 50, 150);
	}
	if (PManager && PManager->Cash) {
		if (ImGui::InputInt("BP", &currency, 1000, 10000))
			PManager->Cash = currency;

		if (ImGui::Checkbox("Disable taking Damage", &bDisableTakingDamage)) {
			if (P10000) {
				if (bDisableTakingDamage) {
					if (P10000) {
						if (!bHaveOriginalVMT) {
							PTenThousandVMT = PTenThousandOneVT.GetReplacedVTablePointer(P10000);
							bHaveOriginalVMT = true;
						}
						PTenThousandVMT = PTenThousandOneVT.GetReplacedVTablePointer(P10000);
						oTakeDamage = (tTakeDamage)PTenThousandOneVT.HookVMT(EPTenThousandVTable::TakeDamage, (uintptr_t)myTakeDamage);
					}
				}
				else {
					if (P10000) {
						if (bHaveOriginalVMT) {
							PTenThousandOneVT.Restore(EPTenThousandVTable::TakeDamage);
						}
					}
				}
			}
		}
		if (P10000) {
			if (ImGui::Button("Randomize Raiden Body")) {
				RandomizeRaiden(P10000);
			}
		}
	}
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

		ImGui_ImplWin32_Init(wnd);
		ImGui_ImplDX9_Init(pDevice);
		MouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, 0, GetCurrentThreadId());
		init = true;
	}

	if (bShowOverlay) {
		P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
		PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);

		if (*gameDrawsCursor == false) {
			/*ImGuiIO& io = ImGui::GetIO();
			io.MouseDrawCursor = true;*/
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		// Create & handle all the Overlay controls
		OverlayLogic();
		
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	//std::cout << "[+] Execution of hooked function successfull.\n";
	LeaveCriticalSection(&critsecHook);
	return oEndScene(pDevice);
}

void PrepareDisco() {
	while (true) {
		std::cout << "Are you ready to party?\n";
		std::cout << "Starting in " << partytimer / 1000 << "...\n";
		Sleep(1000);
		if (partytimer == 0)
			break;
		partytimer -= 1000;
		system("cls");
	}
	return;
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

	MH_Initialize();
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::EndSceneIndex], &Hooked_EndScene, &oEndScene);
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::ResetIndex], &Hooked_Reset, &oResetFunction);
	MH_CreateHookEx((LPVOID)(BaseAddress + 0xA39380), &hkWriteBodyPartData, &oWriteBodyPartData);
	//MH_CreateHookEx((LPVOID)SetCursorPos, &mySetCursorPos, &oSetCursorPosition);
	MH_QueueEnableHook(MH_ALL_HOOKS);

	while (wnd == NULL)
		wnd = **(HWND**)(BaseAddress + 0x9F8DF5);
	oWndProc = (WNDPROC)SetWindowLongPtr(wnd, GWL_WNDPROC, (LONG_PTR)WndProc);
	InitializeCriticalSection(&critsecHook);
	MH_ApplyQueued();

	// This is the address where the game stores the value for whether to show the mouse cursor or not
	gameDrawsCursor = *(bool**)(BaseAddress + 0x9F8CF6);

	system("cls");
	//std::cout << std::hex << *(uintptr_t*)(BaseAddress + 0x19C1490) << "\n";
	P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
	PManager = (PlayerManager*)* (uintptr_t*)(BaseAddress + 0x17EA100);

	
	while(!P10000)
		P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);

	InitializeCriticalSection(&critsec);

	while (true)
	{
		if (TryEnterCriticalSection(&critsec))
		{
			try {
				if (P10000 && P10000->ArmorMeshes)
				{
					if (bEnableDisco) {
						if (P10000->ArmorMeshes->ArmorGlowDiscoR) {
							P10000->ArmorMeshes->ArmorGlowDiscoR = 12.0f;
							Sleep(discoDelay);
						}
						if (P10000->ArmorMeshes->ArmorGlowDiscoG) {
							P10000->ArmorMeshes->ArmorGlowDiscoG = 12.0f;
							Sleep(discoDelay);
						}
						if (P10000->ArmorMeshes->ArmorGlowDiscoB) {
							P10000->ArmorMeshes->ArmorGlowDiscoB = 12.0f;
							Sleep(discoDelay);
						}
						if (P10000->ArmorMeshes->ArmorGlowDiscoAlpha) {
							P10000->ArmorMeshes->ArmorGlowDiscoAlpha = 12.0f;
							Sleep(discoDelay);
						}
					}
				}
				else
					P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
				if (!PManager)
					PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);
				else {
					currency = PManager->Cash;
				}
			}
			catch(...){}
			LeaveCriticalSection(&critsec);
		}
		if (GetAsyncKeyState(VK_END) && 1) {
			bEnableDisco = !bEnableDisco;
			while (GetAsyncKeyState(VK_END) && 1)
				Sleep(10);
			if(bEnableDisco)
				PrepareDisco();
			else {
				system("cls");
				std::cout << "party is over :<\n";
				partytimer = 3000;
			}
		}
		/*if (GetAsyncKeyState(VK_CONTROL) && 1) {
			bEnableDisco = false;
			break;
		}*/
	}

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


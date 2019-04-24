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
#pragma comment(lib, "libMinHook.x86.lib")

#include "MGR.h"
#include "Configs.h"

uintptr_t BaseAddress;
PTenThousand* P10000;
PTenThousand* LastTargetedEnemy;

PlayerManager* PManager;
VRGBA* vRaidenColors = new VRGBA;

CRITICAL_SECTION critsecHook;
CRITICAL_SECTION critsec;
static bool init = false;
bool bShowOverlay = false;
static bool bDisableTakingDamage = false;
static bool bHaveOriginalVMT = false;
int partytimer = 3000;
int currency = 0;

float fCameraHeigthOffset;

int discoDelay = 50;
bool* gameDrawsCursor = NULL;

static const char* selectedPreset = { 0 };

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

static IDirect3DDevice9* d3ddevice;
static D3DPRESENT_PARAMETERS d3dpparams;

// WndProc hook to control showing/hiding of the Overlay
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (GetAsyncKeyState(VK_PAUSE) & 1)
	{
		while (GetAsyncKeyState(VK_PAUSE) & 1)
			Sleep(10);

		bShowOverlay ^= 1;

		// Refresh Presets on Overlay-Toggle
		Configs::GetAvailablePresets();
	}
	
	if (bShowOverlay && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

LONG OldMouseX = NULL;
LONG OldMouseY = NULL;

typedef BOOL(WINAPI* SetCursorPosition)(int X, int Y);
SetCursorPosition oSetCursorPosition = NULL;
BOOL WINAPI hkSetCursorPos(int X, int Y) {
	OldMouseX = X;
	OldMouseY = Y;
	if (bShowOverlay) {
		return 1;
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
		ScreenToClient(GetForegroundWindow(), lpPoint);
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

// 0 = Disabled; 1 = Disco; 2 = Solid
static int iColorsChoice = 0;

// Moved all the Overlay-controls related code in here
void OverlayLogic() {
	// Overlay Window Title and optional settings/flags
	ImGui::Begin("MGR Things - by c0dycode :)");// , 0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Character Colors:");
	ImGui::RadioButton("Disabled", &iColorsChoice, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Disco", &iColorsChoice, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Solid", &iColorsChoice, 2);

	// Restore default colors, jic they have been changed
	if (iColorsChoice == 0) {
		vRaidenColors->R = 1.0f;
		vRaidenColors->G = 1.0f;
		vRaidenColors->B = 1.0f;
		vRaidenColors->A = 1.0f;
	}

	// If Colorselection is not Disabled
	if (iColorsChoice >= 1) {
		ImGui::BeginGroup();
		ImGui::SliderFloat("Red", &vRaidenColors->R, 1.0f, 5.0f);
		ImGui::SliderFloat("Green", &vRaidenColors->G, 1.0f, 5.0f);
		ImGui::SliderFloat("Blue", &vRaidenColors->B, 1.0f, 5.0f);
		//ImGui::SliderFloat("Alpha", &vRaidenColors->A, 1.0f, 20.0f);
		ImGui::EndGroup();

		if (iColorsChoice == 1) {
			ImGui::Separator();
			ImGui::SliderInt("Delay", &discoDelay, 50, 150);
		}
	}
	// Only display if we have a valid pointer to the PlayerManager class
	if (PManager && PManager->Cash) {
		ImGui::Separator();
		if (ImGui::InputInt("BP", &currency, 1000, 10000))
			PManager->Cash = currency;

		// TODO: Disabled until I can figure out a way to re-apply the hooks

		//ImGui::Separator();
		//if (ImGui::Checkbox("Disable taking Damage", &bDisableTakingDamage)) {
		//	if (P10000) {
		//		if (bDisableTakingDamage) {
		//			if (P10000) {
		//				if (!bHaveOriginalVMT) {
		//					PTenThousandVMT = PTenThousandOneVT.GetReplacedVTablePointer(P10000);
		//					bHaveOriginalVMT = true;
		//				}
		//				PTenThousandVMT = PTenThousandOneVT.GetReplacedVTablePointer(P10000);
		//				// Need to figure out a way to re-apply this, since it's being replaced by the default one after a cutscene for example
		//				oTakeDamage = (tTakeDamage)PTenThousandOneVT.HookVMT(EPTenThousandVTable::TakeDamage, (uintptr_t)myTakeDamage);
		//			}
		//		}
		//		else {
		//			if (P10000) {
		//				if (bHaveOriginalVMT) {
		//					PTenThousandOneVT.Restore(EPTenThousandVTable::TakeDamage);
		//				}
		//			}
		//		}
		//	}
		//}

		// Only display if we have a valid pointer to class
		if (P10000) {
			ImGui::Separator();
			if (ImGui::Button("Randomize Raiden Body")) {
				RandomizeRaiden(P10000);
			}
			ImGui::SameLine();
			if (ImGui::Button("UN-Randomize Raiden Body")) {
				UnRandomizeRaiden(P10000);
			}

			// Dropdownlist with all the available saved presets
			// From: https://github.com/ocornut/imgui/issues/1658
			if (ImGui::BeginCombo("Presets", selectedPreset)) {
				for (int n = 0; n < Configs::presetFiles.size(); n++)
				{
					bool is_selected = (selectedPreset == Configs::presetFiles.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
					if (ImGui::Selectable(Configs::presetFiles.at(n).c_str(), is_selected)) {
						selectedPreset = Configs::presetFiles.at(n).c_str();
						if(selectedPreset && strlen(selectedPreset) > 0) 
							Configs::LoadRandomizer(selectedPreset, P10000->RaidenPartCollection);
					}
						if (is_selected)
							ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
				ImGui::EndCombo();
			}

			// Button to save the current Preset
			// From https://github.com/ocornut/imgui/issues/331#issuecomment-140056044
			if (ImGui::Button("Save Current Preset")) {
				// If a preset is selected, show the confirmation popup
				if (selectedPreset && strlen(selectedPreset) > 0)
					ImGui::OpenPopup("PresetConfirmationDialog");
				else
					ImGui::OpenPopup("SavePreset");
			}

			// Popupwindow to save to a new Preset
			if (ImGui::BeginPopupModal("SavePreset")) {
				static char str0[128] = "";
				ImGui::InputText("Preset Name:", str0, IM_ARRAYSIZE(str0));
				if (ImGui::Button("Save")) {
					Configs::SaveRandomizer(str0, P10000->RaidenPartCollection);
					Configs::GetAvailablePresets();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			// ConfirmationPopup if the user wants to overwrite the currently selected Preset
			if (ImGui::BeginPopupModal("PresetConfirmationDialog", 0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("Are you sure you want to overwrite the selected Preset?");
				if (ImGui::Button("Yes")) {
					Configs::SaveRandomizer(selectedPreset, P10000->RaidenPartCollection);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}
	}
	ImGui::Separator();

	// Slider to adjust Camera Heigth
	if (ImGui::SliderFloat("Camera Heigth", &fCameraHeigthOffset, 0.1f, 4.0f)) {
		DWORD old;
		VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, PAGE_READWRITE, &old);
		**(float**)(BaseAddress + 0x6C0C7E + 2) = fCameraHeigthOffset;
		VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, PAGE_READONLY, &old);
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
		//io.ConfigFlags &= ImGuiConfigFlags_NavEnableSetMousePos;
		ImGui_ImplWin32_Init(wnd);
		ImGui_ImplDX9_Init(pDevice);
		MouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, 0, GetCurrentThreadId());
		init = true;
	}

	if (bShowOverlay) {
		P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
		LastTargetedEnemy = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x17EA8C8);
		PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);

		if (*gameDrawsCursor == false) {
			/*ImGuiIO& io = ImGui::GetIO();
			io.MouseDrawCursor = true;*/
			/**gameDrawsCursor ^= true;*/
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

DWORD WINAPI MainThread(LPVOID param)
{
	// Create a Console window // use std::cout << "bla\n" to write to it
	DebugHelper::CreateConsole();

	while (!MyD3D9::GetD3D9Device())
	{
		Sleep(200);
	}
	BaseAddress = (uintptr_t)GetModuleHandle(NULL);

	// Read saved Randomizer Presets
	Configs::GetAvailablePresets();

	MH_Initialize();
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::EndSceneIndex], &Hooked_EndScene, &oEndScene);
	MH_CreateHookEx((LPVOID)MyD3D9::d3d9Device[MyDX3DDeviceIndexes::ResetIndex], &Hooked_Reset, &oResetFunction);
	MH_CreateHookEx((LPVOID)(BaseAddress + 0xA39380), &hkWriteBodyPartData, &oWriteBodyPartData);

	//TODO: These kinda work, but not at all at the same time :D
	//MH_CreateHookEx(GetCursorPos, &hkGetCursorPos, &oGetCursorPosition);
	//MH_CreateHookEx(SetCursorPos, &hkSetCursorPos, &oSetCursorPosition);
	MH_QueueEnableHook(MH_ALL_HOOKS);

	while (wnd == NULL)
		wnd = **(HWND**)(BaseAddress + 0x9F8DF5);
	oWndProc = (WNDPROC)SetWindowLongPtr(wnd, GWL_WNDPROC, (LONG_PTR)WndProc);
	InitializeCriticalSection(&critsecHook);
	MH_ApplyQueued();

	fCameraHeigthOffset = **(float**)(BaseAddress + 0x6C0C7E + 2);

	// This is the address where the game stores the value for whether to show the mouse cursor or not
	gameDrawsCursor = *(bool**)(BaseAddress + 0x9F8CF6);

	system("cls");
	//std::cout << std::hex << *(uintptr_t*)(BaseAddress + 0x19C1490) << "\n";
	P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
	LastTargetedEnemy = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x17EA8C8);
	PManager = (PlayerManager*)* (uintptr_t*)(BaseAddress + 0x17EA100);

	InitializeCriticalSection(&critsec);

	while (true)
	{
		/*if (TryEnterCriticalSection(&critsec))
		{*/
			try {
				if (P10000 && P10000->ArmorMeshes)
				{
					if (iColorsChoice == 1) {
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
					if (iColorsChoice == 2) {
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
				P10000 = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x19C1490);
				LastTargetedEnemy = (PTenThousand*) * (uintptr_t*)(BaseAddress + 0x17EA8C8);
				if (!PManager)
					PManager = (PlayerManager*) * (uintptr_t*)(BaseAddress + 0x17EA100);
				else {
					currency = PManager->Cash;
				}
			}
			catch(...){}
			//LeaveCriticalSection(&critsec);
		//}
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


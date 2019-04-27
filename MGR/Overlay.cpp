#include "Overlay.h"
#include <mutex>

static float fJumpHeightModifier = 0.03f;
static float fJumpSpeedModifier = 0.12f;
static float fSprintSpeedModifier = 0.001f;

static bool bJumpSpeedModified = false;
static bool bJumpHeightModified = false;
static bool bSprintSpeedModified = false;

namespace Overlay {
	static const char* selectedPreset = { 0 };
	int iColorsChoice = 0;

	std::mutex mtx;	

	// Moved all the Overlay-controls related code in here
	void OverlayLogic() {
		// Overlay Window Title and optional settings/flags
		ImGui::Begin("MGR Things - by c0dycode :)", 0, ImGuiWindowFlags_AlwaysAutoResize);

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
						if (selectedPreset && strlen(selectedPreset) > 0)
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
					ImGui::OpenPopup("Preset Confirmation");
				else
					ImGui::OpenPopup("Save Preset");
			}

			// Popupwindow to save to a new Preset
			if (ImGui::BeginPopupModal("Save Preset")) {
				static char str0[128] = "";
				ImGui::InputText("Preset Name:", str0, IM_ARRAYSIZE(str0));
				if (ImGui::Button("Save")) {
					Configs::SaveRandomizer(str0, P10000->RaidenPartCollection);
					Configs::GetAvailablePresets();
					selectedPreset = Configs::presetFiles.at(std::distance(Configs::presetFiles.begin(), std::find(Configs::presetFiles.begin(), Configs::presetFiles.end(), str0))).c_str();

					// Reset the last presetname that was entered
					memset(str0, 0, 128);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			// ConfirmationPopup if the user wants to overwrite the currently selected Preset
			if (ImGui::BeginPopupModal("Preset Confirmation", 0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
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
		
		ImGui::Separator();

		// Slider to adjust Camera Height
		if (ImGui::SliderFloat("Camera Height", &fCameraHeigthOffset, 0.1f, 4.0f)) {
			DWORD old;
			VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, PAGE_EXECUTE_READWRITE, &old);
			**(float**)(BaseAddress + 0x6C0C7E + 2) = fCameraHeigthOffset;
			VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, old, &old);
		}

		mtx.lock();
		
		// Slider to adjust Jump Height
		if (ImGui::SliderFloat("Jump Height", &fJumpHeightModifier, 0.03f, 0.3f)) {
			if (!bJumpHeightModified) {
				DWORD old;
				VirtualProtect((LPVOID)(BaseAddress + 0x7BF8D2 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
				*(float**)(BaseAddress + 0x7BF8D2 + 2) = &fJumpHeightModifier;
				VirtualProtect((LPVOID)(BaseAddress + 0x7BF8D2 + 2), 4, old, &old);
				bJumpHeightModified = true;
			}
		}
		
		// Slider to adjust Jump Speed
		if (ImGui::SliderFloat("Jump Speed", &fJumpSpeedModifier, 0.12f, 0.8f)) {
			if (!bJumpSpeedModified) {
				DWORD old;
				VirtualProtect((LPVOID)(BaseAddress + 0x7BFCEF + 2), 4, PAGE_EXECUTE_READWRITE, &old);
				*(float**)(BaseAddress + 0x7BFCEF + 2) = &fJumpSpeedModifier;
				VirtualProtect((LPVOID)(BaseAddress + 0x7BFCEF + 2), 4, old, &old);
				bJumpSpeedModified = true;
			}
		}

		// Slider to adjust Sprint Speed; not working yet
		/*if (ImGui::SliderFloat("Sprint Speed", &fSprintSpeedModifier, 0.0f, 0.8f)) {
			if (!bSprintSpeedModified) {
				DWORD old;
				VirtualProtect((LPVOID)(BaseAddress + 0x7DD6D9 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
				*(float**)(BaseAddress + 0x7DD6D9 + 2) = &fSprintSpeedModifier;
				VirtualProtect((LPVOID)(BaseAddress + 0x7DD6D9 + 2), 4, old, &old);
				bSprintSpeedModified = true;
			}
		}*/
		mtx.unlock();

// Not sure if this is something that people might actually want lol
#if _DEBUG
		CheatMenu();
		if (ImGui::Button("Cheat Menu")) {
			ImGui::OpenPopup("Cheat Menu");
		}
#endif
	}

	// Popupmenu for Cheat-Like options
	void CheatMenu() {

		static bool bAlwaysNoDamageTakenStat = false;
		// Checkbox to always 
		if (ImGui::BeginPopupModal("Cheat Menu", 0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::Checkbox("Always No Damage Taken Stat", &bAlwaysNoDamageTakenStat)) {
				DWORD oldProtect;
				if (bAlwaysNoDamageTakenStat) {					
					VirtualProtect((LPVOID)(BaseAddress + 0x81B488), 2, PAGE_EXECUTE_READWRITE, &oldProtect);
					// If equal
					if (memcmp((LPVOID)(BaseAddress + 0x81B488), "\x74\x05", 2) == 0)
						memcpy((LPVOID)(BaseAddress + 0x81B488), "\xEB\x05", 2);
					VirtualProtect((LPVOID)(BaseAddress + 0x81B488), 2, oldProtect, &oldProtect);
				}
				else {
					VirtualProtect((LPVOID)(BaseAddress + 0x81B488), 2, PAGE_EXECUTE_READWRITE, &oldProtect);
					// If equal
					if (memcmp((LPVOID)(BaseAddress + 0x81B488), "\xEB\x05", 2) == 0)
						memcpy((LPVOID)(BaseAddress + 0x81B488), "\x74\x05", 2);
					VirtualProtect((LPVOID)(BaseAddress + 0x81B488), 2, oldProtect, &oldProtect);
				}
			}

			if (PManager && PManager->Cash) {
				ImGui::Separator();
				if (ImGui::InputInt("BP", &currency, 1000, 10000))
					PManager->Cash = currency;
			}

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
			
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}
}

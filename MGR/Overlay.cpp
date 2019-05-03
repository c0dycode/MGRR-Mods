#include "Overlay.h"
#include "LogWindow.h"
#include <map>
#include <mutex>

static float fJumpHeightModifier	= 0.03f;
static float fJumpSpeedModifier		= 0.12f;
static float fSprintSpeedModifier	= 0.001f;

static bool bJumpSpeedModified		= false;
static bool bJumpHeightModified		= false;
static bool bSprintSpeedModified	= false;
static bool bIsRainbowActive		= false;
static bool bShowLogWindow			= false;

std::map<EArmorSkin, std::string> ArmorToLoad =
{
	{EArmorSkin::CustomBody, "Custom Body"},
	{EArmorSkin::CustomBodyBlue, "Custom Body Blue"},
	{EArmorSkin::CustomBodyRed, "Custom Body Red"},
	{EArmorSkin::CustomBodyYellow, "Custom Body Yellow"},
	{EArmorSkin::CustomBodyDesperado, "Custom Body Desperado"},
	{EArmorSkin::Suit, "Suit"},
	{EArmorSkin::Mariachi, "Mariachi"},
	{EArmorSkin::StandardBody, "Standard Body"},
	{EArmorSkin::OriginalBody, "Original Body"},
	{EArmorSkin::GrayFox, "Gray Fox"},
	{EArmorSkin::WhiteArmor, "White Armor"},
	{EArmorSkin::InfernoArmor, "Inferno Armor"},
	{EArmorSkin::CommandoArmor, "Commando Armor"}
};

std::vector<std::string> ArmorNames = { "" };

using SetArmorSkin = int(__stdcall*)(EArmorSkin skinToSet);
SetArmorSkin oSetArmorSkin;

using SetArmorSkin2 = int(__stdcall*)();
SetArmorSkin2 sas2;

namespace Overlay {
	const char* selectedPreset = { "" };
	const char* selectedArmor = { "" };
	int iColorsChoice = 0;

	bool bRandomizeTargetedEnemy = false;

	uint32_t LastEnemyRandomized = 0;

	std::mutex mtx;

	void CreateVectOfArmor() {
		for (auto it = ArmorToLoad.begin(); it != ArmorToLoad.end(); ++it) {
			ArmorNames.push_back(it->second);
		}

		// Assign the Functionpointer
		oSetArmorSkin = ((SetArmorSkin)(BaseAddress + 0x5C4F20));
		sas2 = ((SetArmorSkin2)(BaseAddress + 0x5C7C20));
	}

	void ReadSelectedArmorFromGame() {
		if (*(EArmorSkin * *)(BaseAddress + 0x5C4F59 + 2) && strlen(selectedArmor) <= 0) {
			for (auto it = ArmorToLoad.begin(); it != ArmorToLoad.end(); ++it) {
				if (it->first == **(EArmorSkin**)(BaseAddress + 0x5C4F59 + 2))
					selectedArmor = it->second.c_str();
			}
		}
	}

	void SetArmor(const char* armorName) {
		for (auto it = ArmorToLoad.begin(); it != ArmorToLoad.end(); ++it) {
			if (strstr(it->second.c_str(), armorName))
				if (oSetArmorSkin) {
					oSetArmorSkin(it->first);
					InterlockedExchange((*(uintptr_t**)(BaseAddress + 0x5C7C45 + 1)), static_cast<uint32_t>(it->first));
					InterlockedExchange((*(uintptr_t**)(BaseAddress + 0x5C7C5D + 1)), static_cast<uint32_t>(it->first));
					sas2();
				}
		}
	}

	//static bool bTerminateRainbow;
	//// Does the Rainbow work in a different thread
	//void EnableRainbow() {
	//	bIsRainbowActive = true;
	//	
	//	while (!bTerminateRainbow) {
	//		
	//		if (mtx.try_lock()) {
	//			
	//		

	//			vRaidenColors->R = r;
	//			vRaidenColors->G = g;
	//			vRaidenColors->B = b;
	//			mtx.unlock();
	//		}			
	//		Sleep(10);
	//	}
	//	bIsRainbowActive = false;
	//}

	// Moved all the Overlay-controls related code in here
	void OverlayLogic() {
		// Make sure that the Rainbow is being terminated as soon as possible when no longer selected
		/*if (bIsRainbowActive && iColorsChoice != 3)
			bTerminateRainbow = true;*/

		// Overlay Window Title and optional settings/flags
		ImGui::Begin("MGR Things - by c0dycode :)", 0, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("Character Colors:");
		ImGui::RadioButton("Disabled", &iColorsChoice, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Disco", &iColorsChoice, 1);
		ImGui::SameLine();
		ImGui::RadioButton("Solid", &iColorsChoice, 2);
		/*ImGui::SameLine();
		ImGui::RadioButton("Rainbow", &iColorsChoice, 3);*/

		// Restore default colors, jic they have been changed
		if (iColorsChoice == 0) {
			vRaidenColors->R = 1.0f;
			vRaidenColors->G = 1.0f;
			vRaidenColors->B = 1.0f;
			vRaidenColors->A = 1.0f;
		}

		if (mtx.try_lock()) {
			// If Colorselection is not Disabled
			if (iColorsChoice >= 1 && iColorsChoice < 3) {
				ImGui::BeginGroup();
				ImGui::SliderFloat("Red", &vRaidenColors->R, 1.0f, 5.0f);
				ImGui::SliderFloat("Green", &vRaidenColors->G, 1.0f, 5.0f);
				ImGui::SliderFloat("Blue", &vRaidenColors->B, 1.0f, 5.0f);
				ImGui::SliderFloat("Transparency", &vRaidenColors->A, 0.1f, 1.0f);
				ImGui::EndGroup();

				if (iColorsChoice == 1) {
					ImGui::Separator();
					ImGui::SliderInt("Delay", &discoDelay, 50, 150);
				}
			}
			mtx.unlock();
		}
		// Rainbow choice
		/*if (iColorsChoice == 3 && !bIsRainbowActive) {
			std::thread t (EnableRainbow);
			t.detach();
		}*/

		// Only display if we have a valid pointer to class
		if (P10000) {
			if (P10000->MeshesCount > 0) {
				if (ImGui::Button("Randomize Character Colors")) {
					RandomizeColors(P10000, 1.0f, 2.5f);
				}
				ImGui::SameLine();
				if (ImGui::Button("Un-Randomize Character Colors")) {
					UnRandomizeColors(P10000);
				}
			}
			if (PlWig && PlWig->MeshesCount > 0) {
				if (ImGui::Button("Randomize Hair Color")) {
					RandomizeColors(PlWig, 1.0f, 1.5);
				}
				ImGui::SameLine();
				if (ImGui::Button("Un-Randomize Hair Color")) {
					UnRandomizeColors(PlWig);
				}
			}

			if (PlSwordSheath && PlSwordSheath->MeshesCount > 0) {
				if (ImGui::Button("Randomize Sheath Color")) {
					RandomizeColors(PlSwordSheath, 1.0f, 3.0f);
				}
				ImGui::SameLine();
				if (ImGui::Button("Un-Randomize Sheath Color")) {
					UnRandomizeColors(PlSwordSheath);
				}
			}

			ImGui::Separator();
			if (ImGui::Button("Randomize Character Body")) {
				RandomizeBody(P10000);
			}
			ImGui::SameLine();
			if (ImGui::Button("UN-Randomize Character Body")) {
				UnRandomizeBody(P10000);
				selectedPreset = "";
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
							Configs::LoadRandomizer(selectedPreset, P10000->PartCollection, vRaidenColors);
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
			if (ImGui::BeginPopupModal("Save Preset", 0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
				static char str0[128] = "";
				ImGui::InputText("Preset Name:", str0, IM_ARRAYSIZE(str0));
				if (ImGui::Button("Save")) {
					Configs::SaveRandomizer(str0, P10000->PartCollection, vRaidenColors);
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
					Configs::SaveRandomizer(selectedPreset, P10000->PartCollection, vRaidenColors);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}

		// Dropdownlist with all the available armornames
		//if (ImGui::BeginCombo("Armors", selectedArmor)) {
		//	for (int n = 0; n < ArmorNames.size(); n++)
		//	{
		//		bool is_selected = (selectedArmor == ArmorNames.at(n).c_str()); // You can store your selection however you want, outside or inside your objects
		//		if (ImGui::Selectable(ArmorNames.at(n).c_str(), is_selected)) {
		//			selectedArmor = ArmorNames.at(n).c_str();
		//			if (selectedArmor && strlen(selectedArmor) > 0)
		//				SetArmor(selectedArmor);
		//		}
		//		if (is_selected)
		//			ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		//	}
		//	ImGui::EndCombo();
		//}
		
		ImGui::Separator();

		ImGui::Checkbox("Randomize Targeted Enemy", &bRandomizeTargetedEnemy);

		ImGui::Separator();



		// Slider to adjust Camera Height
		if (ImGui::SliderFloat("Camera Height", &fCameraHeigthOffset, 0.1f, 4.0f)) {
			DWORD old;
			VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, PAGE_EXECUTE_READWRITE, &old);
			**(float**)(BaseAddress + 0x6C0C7E + 2) = fCameraHeigthOffset;
			VirtualProtect(*(float**)(BaseAddress + 0x6C0C7E + 2), 4, old, &old);
		}

		if (mtx.try_lock()) {

			// Slider to adjust Jump Height
			if (ImGui::SliderFloat("Jump Height", &fJumpHeightModifier, 0.03f, 0.3f)) {
				if (!bJumpHeightModified) {
					DWORD old;

					#pragma region Raiden
					VirtualProtect((LPVOID)(BaseAddress + 0x7BF8D2 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					*(float**)(BaseAddress + 0x7BF8D2 + 2) = &fJumpHeightModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x7BF8D2 + 2), 4, old, &old);
					#pragma endregion

					#pragma region Sam
					VirtualProtect((LPVOID)(BaseAddress + 0x46A5A4 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					*(float**)(BaseAddress + 0x46A5A4 + 2) = &fJumpHeightModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x46A5A4 + 2), 4, old, &old);
					#pragma endregion

					#pragma region BladeWolf
					VirtualProtect((LPVOID)(BaseAddress + 0x4AC7FE + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					*(float**)(BaseAddress + 0x4AC7FE + 2) = &fJumpHeightModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x4AC7FE + 2), 4, old, &old);
					#pragma endregion

					bJumpHeightModified = true;
				}
			}

			// Slider to adjust Jump Speed
			if (ImGui::SliderFloat("Jump Speed", &fJumpSpeedModifier, 0.12f, 0.8f)) {
				if (!bJumpSpeedModified) {
					DWORD old;

					#pragma region Raiden
					VirtualProtect((LPVOID)(BaseAddress + 0x7BFCEF + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					// Remove check whether we are sprinting or not // without this, sprint-jump is not affected by this mod
					if (memcmp((LPVOID)(BaseAddress + 0x7BFC51), "\x74\x5F", 2) == 0)
						memcpy((LPVOID)(BaseAddress + 0x7BFC51), "\xEB\x5F", 2);
					*(float**)(BaseAddress + 0x7BFCEF + 2) = &fJumpSpeedModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x7BFCEF + 2), 4, old, &old);
					#pragma endregion

					#pragma region Sam
					VirtualProtect((LPVOID)(BaseAddress + 0x46A914 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					// Remove check whether we are sprinting or not // without this, sprint-jump is not affected by this mod
					if (memcmp((LPVOID)(BaseAddress + 0x46A8FD), "\x75\x15", 2) == 0)
						memcpy((LPVOID)(BaseAddress + 0x46A8FD), "\xEB\x15", 2);
					*(float**)(BaseAddress + 0x46A914 + 2) = &fJumpSpeedModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x46A914 + 2), 4, old, &old);
					#pragma endregion

					#pragma region BladeWolf
					VirtualProtect((LPVOID)(BaseAddress + 0x4ACA75 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					// Remove check whether we are sprinting or not // without this, sprint-jump is not affected by this mod
					if (memcmp((LPVOID)(BaseAddress + 0x4ACA4E), "\x74\x25", 2) == 0)
						memcpy((LPVOID)(BaseAddress + 0x4ACA4E), "\xEB\x25", 2);
					*(float**)(BaseAddress + 0x4ACA75 + 2) = &fJumpSpeedModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x4ACA75 + 2), 4, old, &old);
					#pragma endregion

					bJumpSpeedModified = true;
				}
			}

			// Slider to adjust Sprint Speed; not working yet
			/*if (ImGui::SliderFloat("Sprint Speed", &fSprintSpeedModifier, 0.001f, 0.8f)) {
				if (!bSprintSpeedModified) {
					DWORD old;
					VirtualProtect((LPVOID)(BaseAddress + 0x7DD6E7 + 2), 4, PAGE_EXECUTE_READWRITE, &old);
					*(float**)(BaseAddress + 0x7DD6E7 + 2) = &fSprintSpeedModifier;
					VirtualProtect((LPVOID)(BaseAddress + 0x7DD6E7 + 2), 4, old, &old);
					bSprintSpeedModified = true;
				}
			}*/
			mtx.unlock();
		}

		// Not sure if this is something that people might actually want lol
		ImGui::Separator();
		CheatMenu();
		if (ImGui::Button("Cheat Menu")) {
			ImGui::OpenPopup("Cheat Menu");
		}

		if (ImGui::Button("Log")) {
			bShowLogWindow ^= 1;
			
		}
		if(bShowLogWindow)
			AppLog::ShowAppLog(&bShowLogWindow);
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
					//PManager->Cash = currency;
					InterlockedExchange(&PManager->Cash,currency);
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

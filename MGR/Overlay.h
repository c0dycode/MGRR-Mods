#ifndef OVERLAY_H
#define OVERLAY_H

#include "GlobalVariables.h"
#include "ImGui/imgui.h"
#include "Configs.h"
#include <vector>

namespace Overlay{
	extern const char* selectedPreset;
	extern const char* selectedArmor;

	// 0 = Disabled; 1 = Disco; 2 = Solid; 3 = Rainbow
	extern int iColorsChoice;

	// Randomize Targeted Enemy
	extern bool bRandomizeTargetedEnemy;

	// "ID"? of last targeted enemy
	extern uint32_t LastEnemyRandomized;

	// Popupmenu for Cheat-Like options
	void CheatMenu();

	// Moved all the Overlay-controls related code in here
	void OverlayLogic();

	// Creates a std::vector<std::string> of all the armor names
	void CreateVectOfArmor();

	// Reads the selected Armor from the game directly
	void ReadSelectedArmorFromGame();

	// Try and set the armor ingame
	void SetArmor(const char* armorName);

	// Does the Rainbow work in a different thread
	//void EnableRainbow();
};

#endif
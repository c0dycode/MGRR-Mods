#pragma once
#ifndef CONFIGS_H
#define CONFIGS_H

#include <experimental/filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "MGR.h"

namespace Configs {

	extern std::vector<std::string> presetFiles;

	// Save the current Configuration in presetName
	void SaveRandomizer(const char* presetName, RaidenParts* partsToSave);

	// Restore Saved configuration to Parts
	void LoadRandomizer(const char* presetName, RaidenParts* partsToLoad);

	//std::vector<std::string> presetFiles = {""};

	void GetAvailablePresets();
}

#endif
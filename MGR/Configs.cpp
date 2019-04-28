#include "Configs.h"

namespace Configs {

	// Save the current Configuration in presetName
	void SaveRandomizer(const char* presetName, RaidenParts* partsToSave, VRGBA* colors) {

		// Append fileextension, because the GUI will only ask for a name and not a filename
		std::string presetFilename = std::string(presetName).append(".txt");

		std::ofstream presetFile;
		std::string filePath = std::string("..\\Plugins\\Presets\\").append(presetFilename);
		presetFile.open(filePath);

		#pragma region Model Parts

		presetFile << "[Chest]\n";
		presetFile << "X=" << partsToSave->Chest->X << "\n";
		presetFile << "Y=" << partsToSave->Chest->Y << "\n";
		presetFile << "Z=" << partsToSave->Chest->Z << "\n";

		presetFile << "[Chest2]\n";
		presetFile << "X=" << partsToSave->Chest2->X << "\n";
		presetFile << "Y=" << partsToSave->Chest2->Y << "\n";
		presetFile << "Z=" << partsToSave->Chest2->Z << "\n";

		presetFile << "[Chest3]\n";
		presetFile << "X=" << partsToSave->Chest3->X << "\n";
		presetFile << "Y=" << partsToSave->Chest3->Y << "\n";
		presetFile << "Z=" << partsToSave->Chest3->Z << "\n";

		presetFile << "[LeftLowerLeg]\n";
		presetFile << "X=" << partsToSave->LeftLowerLeg->X << "\n";
		presetFile << "Y=" << partsToSave->LeftLowerLeg->Y << "\n";
		presetFile << "Z=" << partsToSave->LeftLowerLeg->Z << "\n";

		presetFile << "[RightLowerLeg]\n";
		presetFile << "X=" << partsToSave->RightLowerLeg->X << "\n";
		presetFile << "Y=" << partsToSave->RightLowerLeg->Y << "\n";
		presetFile << "Z=" << partsToSave->RightLowerLeg->Z << "\n";

		presetFile << "[Head2]\n";
		presetFile << "X=" << partsToSave->Head2->X << "\n";
		presetFile << "Y=" << partsToSave->Head2->Y << "\n";
		presetFile << "Z=" << partsToSave->Head2->Z << "\n";
		#pragma endregion

		#pragma region Colors
		presetFile << "[Colors]\n";
		presetFile << "R=" << colors->R << "\n";
		presetFile << "G=" << colors->G << "\n";
		presetFile << "B=" << colors->B << "\n";
		presetFile << "A=" << colors->A << "\n";
		#pragma endregion 

		presetFile.close();
	}

	// Restore Saved configuration to Parts
	void LoadRandomizer(const char* presetName, RaidenParts* partsToLoad, VRGBA* colors) {
		std::string presetFilename = std::string(presetName).append(".txt");

		std::string filePath = std::string("..\\Plugins\\Presets\\").append(presetFilename);

		// Do nothing if file does not exist
		if (!std::experimental::filesystem::exists(filePath))
			return;

		std::ifstream is_file(filePath.c_str());

		std::string line;
		cPart* current = { 0 };
		while (std::getline(is_file, line))
		{
			if (line == "[Chest]")
				current = partsToLoad->Chest;
			if (line == "[Chest2]")
				current = partsToLoad->Chest2;
			if (line == "[Chest3]")
				current = partsToLoad->Chest3;
			if (line == "[LeftLowerLeg]")
				current = partsToLoad->LeftLowerLeg;
			if (line == "[RightLowerleg]")
				current = partsToLoad->RightLowerLeg;
			if (line == "[Head2]")
				current = partsToLoad->Head2;

			#pragma region Colors
			if (line.find("R=") != std::string::npos) {
				line.replace(0, 2, "");
				colors->R = strtof(line.c_str(), NULL);
			}
			if (line.find("G=") != std::string::npos) {
				line.replace(0, 2, "");
				colors->G = strtof(line.c_str(), NULL);
			}
			if (line.find("B=") != std::string::npos) {
				line.replace(0, 2, "");
				colors->B = strtof(line.c_str(), NULL);
			}
			if (line.find("A=") != std::string::npos) {
				line.replace(0, 2, "");
				colors->A = strtof(line.c_str(), NULL);
			}
			#pragma endregion

			if (!current)
				continue;

			if (line.find("X=") != std::string::npos) {
				line.replace(0, 2, "");
				current->X = strtof(line.c_str(), NULL);
				if (current->X != 1.0f)
					current->Freeze = 8;
			}
			if (line.find("Y=") != std::string::npos) {
				line.replace(0, 2, "");
				current->Y = strtof(line.c_str(), NULL);
				if (current->Y != 1.0f)
					current->Freeze = 8;
			}
			if (line.find("Z=") != std::string::npos) {
				line.replace(0, 2, "");
				current->Z = strtof(line.c_str(), NULL);
				if (current->Z != 1.0f)
					current->Freeze = 8;
			}
		}
	}

	std::vector<std::string> presetFiles;

	void GetAvailablePresets() {
		// Create Presets-directory if it doesn't exist already
		if (CreateDirectory(L"..\\Plugins\\Presets", NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			if (std::experimental::filesystem::is_empty("..\\Plugins\\Presets\\"))
				return;

			presetFiles.clear();
			presetFiles.push_back("");
			for (const auto& entry : std::experimental::filesystem::directory_iterator("..\\Plugins\\Presets")) {

				auto path = entry.path().string();
				path.replace(0, 19, "");
				auto fileExtension = path.find(".txt");
				if (fileExtension != std::string::npos)
					path.replace(fileExtension, 4, "");
				presetFiles.push_back(path);
			}
			return;
		}
	}
}

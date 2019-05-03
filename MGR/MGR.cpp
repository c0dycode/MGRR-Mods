#include "MGR.h"
#include "GlobalVariables.h"

static std::map<std::string, VRGBA*> colorMap;

void RandomizeBody(PTenThousand* pTenK) {
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(-2.5f, 2.5f);

	auto partCollection = pTenK->PartCollection;
	if (partCollection) {
#pragma region Chest
		if (partCollection->Chest) {
			partCollection->Chest->X = dis(e);
			partCollection->Chest->Y = dis(e);
			partCollection->Chest->Z = dis(e);
			partCollection->Chest->Freeze = 8;
		}
#pragma endregion
#pragma region Chest2
		if (partCollection->Chest2) {
			partCollection->Chest2->X = dis(e);
			partCollection->Chest2->Y = dis(e);
			partCollection->Chest2->Z = dis(e);
			partCollection->Chest2->Freeze = 8;
		}
#pragma endregion
#pragma region Chest3
		if (partCollection->Chest3) {
			partCollection->Chest3->X = dis(e);
			partCollection->Chest3->Y = dis(e);
			partCollection->Chest3->Z = dis(e);
			partCollection->Chest3->Freeze = 8;
		}
#pragma endregion

#pragma region Legs
		if (partCollection->LeftLowerLeg) {
			partCollection->LeftLowerLeg->X = dis(e);
			partCollection->LeftLowerLeg->Y = dis(e);
			partCollection->LeftLowerLeg->Freeze = 8;
		}
		if (partCollection->RightLowerLeg) {
			partCollection->RightLowerLeg->X = dis(e);
			partCollection->RightLowerLeg->Y = dis(e);
			partCollection->RightLowerLeg->Freeze = 8;
		}
#pragma endregion

#pragma region Head
		if (partCollection->Head2) {
			partCollection->Head2->X = dis(e);
			partCollection->Head2->Y = dis(e);
			partCollection->Head2->Z = dis(e);
			partCollection->Head2->Freeze = 8;
		}
#pragma endregion
	}
}

void UnRandomizeBody(PTenThousand* pTenK)
{
	auto partCollection = pTenK->PartCollection;
	if (partCollection) {
#pragma region Chest
		if (partCollection->Chest) {
			partCollection->Chest->X = 1.0f;
			partCollection->Chest->Y = 1.0f;
			partCollection->Chest->Z = 1.0f;
			partCollection->Chest->Freeze = 0;
		}
#pragma endregion
#pragma region Chest2
		if (partCollection->Chest2) {
			partCollection->Chest2->X = 1.0f;
			partCollection->Chest2->Y = 1.0f;
			partCollection->Chest2->Z = 1.0f;
			partCollection->Chest2->Freeze = 0;
		}
#pragma endregion
#pragma region Chest3
		if (partCollection->Chest3) {
			partCollection->Chest3->X = 1.0f;
			partCollection->Chest3->Y = 1.0f;
			partCollection->Chest3->Z = 1.0f;
			partCollection->Chest3->Freeze = 0;
		}
#pragma endregion

#pragma region Legs
		if (partCollection->LeftLowerLeg) {
			partCollection->LeftLowerLeg->X = 1.0f;
			partCollection->LeftLowerLeg->Y = 1.0f;
			partCollection->LeftLowerLeg->Freeze = 0;
		}
		if (partCollection->RightLowerLeg) {
			partCollection->RightLowerLeg->X = 1.0f;
			partCollection->RightLowerLeg->Y = 1.0f;
			partCollection->RightLowerLeg->Freeze = 0;
		}
#pragma endregion

#pragma region Head
		if (partCollection->Head2) {
			partCollection->Head2->X = 1.0f;
			partCollection->Head2->Y = 1.0f;
			partCollection->Head2->Z = 1.0f;
			partCollection->Head2->Freeze = 0;
		}
#pragma endregion
	}
}

// Randomize the colors of all available meshes in this class
void RandomizeColors(PTenThousand* pTenK, float customLimitMin, float customLimitMax) {
	static std::default_random_engine e;
	std::uniform_real_distribution<> dis(customLimitMin, customLimitMax);

	if (!pTenK)
		return;
	for (int i = 0; i < pTenK->MeshesCount; ++i) {
		if (pTenK->Meshes) {
			pTenK->Meshes[i].GlowR = dis(e);
			pTenK->Meshes[i].GlowG = dis(e);
			pTenK->Meshes[i].GlowB = dis(e);
			pTenK->Meshes[i].Glow2R = dis(e);
			pTenK->Meshes[i].Glow2G = dis(e);
			pTenK->Meshes[i].Glow2B = dis(e);

			std::string name(pTenK->Meshes[i].MeshInfo->MeshTarget);

			if (colorMap.find(name) != colorMap.end()) {
				colorMap[name]->R = pTenK->Meshes[i].GlowR;
				colorMap[name]->G = pTenK->Meshes[i].GlowG;
				colorMap[name]->B = pTenK->Meshes[i].GlowB;
				colorMap[name]->A = pTenK->Meshes[i].Alpha;
			}
			else {
				VRGBA* colorStruct = new VRGBA(pTenK->Meshes[i].GlowR, pTenK->Meshes[i].GlowG, pTenK->Meshes[i].GlowB, pTenK->Meshes[i].Alpha);
				colorMap[name] = colorStruct;
			}
		}
	}
}

void UnRandomizeColors(PTenThousand* pTenK) {
	if (!pTenK)
		return;
	for (int i = 0; i < pTenK->MeshesCount; ++i) {
		if (pTenK->Meshes) {
			pTenK->Meshes[i].GlowR = 1.0f;
			pTenK->Meshes[i].GlowG = 1.0f;
			pTenK->Meshes[i].GlowB = 1.0f;
			pTenK->Meshes[i].Glow2R = 1.0f;
			pTenK->Meshes[i].Glow2G = 1.0f;
			pTenK->Meshes[i].Glow2B = 1.0f;
			//pTenK->Meshes[i].Alpha = 1.0f;

			std::string name(pTenK->Meshes[i].MeshInfo->MeshTarget);
			auto it = colorMap.find(name);
			if (it != colorMap.end()) {
				free(colorMap[name]);
				colorMap.erase(it);
			}
		}
	}
}

bool bColorsNeedRefresh;

void RefreshColorsIfNeeded() {
#pragma region Refresh Colors
	if (bColorsNeedRefresh) {
		if (P10000) {
			while (P10000->MeshesCount == 0)
				Sleep(5);
			for (int i = 0; i < P10000->MeshesCount; ++i) {
				std::string name(P10000->Meshes[i].MeshInfo->MeshTarget);
				if (colorMap.find(name) != colorMap.end()) {
					P10000->Meshes[i].GlowR = colorMap[name]->R;
					P10000->Meshes[i].GlowG = colorMap[name]->G;
					P10000->Meshes[i].GlowB = colorMap[name]->B;
					P10000->Meshes[i].Alpha = colorMap[name]->A;
				}
			}
		}
		if (PlSwordSheath) {
			while (PlSwordSheath->MeshesCount == 0)
				Sleep(5);
			for (int i = 0; i < PlSwordSheath->MeshesCount; ++i) {
				std::string name(PlSwordSheath->Meshes[i].MeshInfo->MeshTarget);
				if (colorMap.find(name) != colorMap.end()) {
					PlSwordSheath->Meshes[i].GlowR = colorMap[name]->R;
					PlSwordSheath->Meshes[i].GlowG = colorMap[name]->G;
					PlSwordSheath->Meshes[i].GlowB = colorMap[name]->B;
					PlSwordSheath->Meshes[i].Alpha = colorMap[name]->A;
				}
			}
		}
		if (PlWig) {
			while (PlWig->MeshesCount == 0)
				Sleep(5);
			for (int i = 0; i < PlWig->MeshesCount; ++i) {
				std::string name(PlWig->Meshes[i].MeshInfo->MeshTarget);
				if (colorMap.find(name) != colorMap.end()) {
					PlWig->Meshes[i].GlowR = colorMap[name]->R;
					PlWig->Meshes[i].GlowG = colorMap[name]->G;
					PlWig->Meshes[i].GlowB = colorMap[name]->B;
					PlWig->Meshes[i].Alpha = colorMap[name]->A;
				}
			}
		}
		bColorsNeedRefresh = false;
	}
#pragma endregion
}
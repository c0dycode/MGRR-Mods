#include "MGR.h"

void RandomizeBody(PTenThousand* pTenK) {
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(-2.5, 2.5);
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

#ifndef OVERLAY_H
#define OVERLAY_H

#include "GlobalVariables.h"
#include "ImGui/imgui.h"
#include "Configs.h"
#include <vector>

namespace Overlay{

	// 0 = Disabled; 1 = Disco; 2 = Solid
	extern int iColorsChoice;

	// Moved all the Overlay-controls related code in here
	void OverlayLogic();

//private:
//	// Ctor
//	Overlay() {
//		iColorsChoice = 0;
//	}
//	//DCtor
//	~Overlay() {}
};

#endif
#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "TexturePositions.h"
#include "SubSystem.h"
#include "Bar.h"
#include "Switch.h"
#include "StatusLight.h"

class SubSystemInstrument: public PanelElement {
public:
	SubSystemInstrument (VESSEL3 *v,SubSystem *subSystem,DWORD x,DWORD y);
	bool Redraw2D (SURFHANDLE tgt,SURFHANDLE src);
	bool ProcessMouse2D (int event, int mx, int my);
  
private:
	SubSystem *ss;
	Switch *sw;
	StatusLight *led;
	Bar *bar; // Pointer to dynamic array
	DWORD xPos,yPos;
	double lastRefreshTime;

	void initializeInstrument();
};




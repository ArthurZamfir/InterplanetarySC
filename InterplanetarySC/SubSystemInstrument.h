#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "TexturePositions.h"
#include "SubSystem.h"
#include "Bar.h"
#include "OperationSwitch.h"
#include "StatusLight.h"

class SubSystemInstrument: public PanelElement {
public:
	SubSystemInstrument (VESSEL3* v,SubSystem* subSystem,DWORD x,DWORD y);
	bool Redraw2D (SURFHANDLE tgt,SURFHANDLE src);
	bool ProcessMouse2D (int event, int mx, int my);
  
private:
	SubSystem* ss_;
	OperationSwitch* sw_;
	StatusLight* led1_;
	StatusLight* led2_;
	std::vector<Bar> bar_;
	DWORD xPos_,yPos_;
	double lastRefreshTime_;
	int attributeCount_;
	bool firstDraw_;
	bool switchedState_;
	Status status_;
	void drawText(std::string text,SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy);
	int getPanelHeight();
};
#pragma once
#include "Instrument.h"
#include "SubSystemLib.h"

class OperationSwitch{
public:
	OperationSwitch (DWORD x,DWORD y,OPERATION_MODE *operationMode);
	bool draw(SURFHANDLE tgt,SURFHANDLE src);

private:
	OPERATION_MODE *om;
	OPERATION_MODE lastOM;
	DWORD xPos,yPos;

	void drawSwitchON();
	void drawSwitchOFF();
};
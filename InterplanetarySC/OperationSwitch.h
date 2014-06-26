#pragma once
#include "Instrument.h"
#include "SubSystemLib.h"

class OperationSwitch{
public:
	OperationSwitch (DWORD x,DWORD y);
	void drawSwitchON(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy);
	void drawSwitchOFF(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy);

private:
	//OPERATION_MODE *om;
	//OPERATION_MODE lastOM;
	DWORD xPos,yPos;

	
};
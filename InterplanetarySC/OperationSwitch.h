#pragma once
#include "Instrument.h"
#include "SubSystemLib.h"

class OperationSwitch{
public:
	OperationSwitch (DWORD x,DWORD y);
	void drawSwitchON(SURFHANDLE tgt,SURFHANDLE src,double dx,double dy);
	void drawSwitchOFF(SURFHANDLE tgt,SURFHANDLE src,double dx,double dy);

private:
	//OPERATION_MODE *om;
	//OPERATION_MODE lastOM;
	DWORD xPos,yPos;

	
};
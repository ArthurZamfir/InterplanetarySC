#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "SubSystemLib.h"

class StatusLight{
public:
	StatusLight (DWORD x,DWORD y,OPERATION_MODE *operationMode);
	bool draw(SURFHANDLE tgt,SURFHANDLE src);

private:
	OPERATION_MODE *om;
	OPERATION_MODE lastOM;
	DWORD xPos,yPos;
};
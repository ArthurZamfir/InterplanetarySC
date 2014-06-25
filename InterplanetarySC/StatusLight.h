#pragma once
#include "Instrument.h"
#include "OrbiterAPI.h"
#include "SubSystemLib.h"

class StatusLight{
public:
	StatusLight (DWORD x,DWORD y);
	void drawStatus(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,OPERATION_MODE op);
	void drawActive(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,bool active);

private:
	DWORD xPos,yPos;
};
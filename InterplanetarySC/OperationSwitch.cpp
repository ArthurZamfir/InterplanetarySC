#include "OperationSwitch.h"
#include "TexturePositions.h"


OperationSwitch::OperationSwitch (DWORD x,DWORD y)
{
   xPos_ = x;
   yPos_ = y;
}

void OperationSwitch::drawSwitchON (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	oapiBlt(tgt,src,xPos_+dx,yPos_+dy,switchOnX0,switchOnY0,switchOnWidth,switchOnHeight);
}

void OperationSwitch::drawSwitchOFF (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	oapiBlt(tgt,src,xPos_+dx,yPos_+dy,switchOffX0,switchOffY0,switchOffWidth,switchOffHeight);
}

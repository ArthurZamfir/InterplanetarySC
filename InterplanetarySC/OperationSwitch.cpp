#include "OperationSwitch.h"
#include "TexturePositions.h"


OperationSwitch::OperationSwitch(DWORD x,DWORD y)
{
   xPos = x;
   yPos = y;
   //om = operationMode;
   //lastOM = *operationMode;
}

////Der switch sollte vielleicht nicht abhängig sein vom OP mode,
////sodass er sich nicht automatisch umschaltet wenn das SS einen
////Fehler erzeugt?! Oder doch?
//bool OperationSwitch::draw(SURFHANDLE tgt,SURFHANDLE src)
//{
//  	if(lastOM == *om)
//		return false;
//	lastOM = *om;
//	switch(*om){
//	case ACTIVE:
//		draw
//	case ACTIVE_WARNING:
//	case ACTIVE_ERROR:
//	case PASSIVE:
//	case PASSIVE_WARNING:
//	case PASSIVE_ERROR:
//	}
//	return true;
//}

void OperationSwitch::drawSwitchON(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	oapiBlt(tgt,src,xPos+dx,yPos+dy,switchOnX0,switchOnY0,switchOnWidth,switchOnHeight);
}

void OperationSwitch::drawSwitchOFF(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	oapiBlt(tgt,src,xPos+dx,yPos+dy,switchOffX0,switchOffY0,switchOffWidth,switchOffHeight);
}

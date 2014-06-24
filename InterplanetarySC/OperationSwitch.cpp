#include "OperationSwitch.h"


OperationSwitch::OperationSwitch(DWORD x,DWORD y,OPERATION_MODE *operationMode)
{
   xPos = x;
   yPos = y;
   om = operationMode;
   lastOM = *operationMode;
}

//Der switch sollte vielleicht nicht abhängig sein vom OP mode,
//sodass er sich nicht automatisch umschaltet wenn das SS einen
//Fehler erzeugt?! Oder doch?
bool OperationSwitch::draw(SURFHANDLE tgt,SURFHANDLE src)
{
  	if(lastOM == *om)
		return false;
	lastOM = *om;
	switch(*om){
	case ACTIVE: 
	case ACTIVE_WARNING:
	case ACTIVE_ERROR:
	case PASSIVE:
	case PASSIVE_WARNING:
	case PASSIVE_ERROR:
	}
}

void OperationSwitch::drawSwitchON()
{
	oapiBlt();
}

void OperationSwitch::drawSwitchOFF()
{
	oapiBlt();
}

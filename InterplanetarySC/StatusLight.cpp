#include "StatusLight.h"

StatusLight::StatusLight (DWORD x,DWORD y,OPERATION_MODE *operationMode)
{
	xPos = x;
	yPos = y;
	om = operationMode;
	lastOM = *operationMode;
}

bool StatusLight::draw(SURFHANDLE tgt,SURFHANDLE src)
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
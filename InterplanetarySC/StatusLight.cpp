#include "StatusLight.h"
#include "TexturePositions.h"

StatusLight::StatusLight (DWORD x,DWORD y)
{
	xPos = x;
	yPos = y;
}

void StatusLight::drawStatus(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,OPERATION_MODE op)
{
	switch(op){
	case ACTIVE:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
		break;
	case ACTIVE_WARNING:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledYellowX0,ledYellowY0,ledWidht,ledHeight);
		break;
	case ACTIVE_ERROR:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledRedX0,ledRedY0,ledWidht,ledHeight);
		break;
	case PASSIVE:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
		break;
	case PASSIVE_WARNING:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledYellowX0,ledYellowY0,ledWidht,ledHeight);
		break;
	case PASSIVE_ERROR:
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledRedX0,ledRedY0,ledWidht,ledHeight);
		break;
	}

}

void StatusLight::drawActive(SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,bool active)
{
	if(active)
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
	else
		oapiBlt(tgt,src,xPos+dx,yPos+dy,ledBlackX0,ledBlackY0,ledWidht,ledHeight);
}
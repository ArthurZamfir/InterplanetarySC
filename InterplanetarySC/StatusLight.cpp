#include "StatusLight.h"
#include "TexturePositions.h"

StatusLight::StatusLight (DWORD x,DWORD y)
{
	xPos_ = x;
	yPos_ = y;
}

void StatusLight::drawStatus (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,Status status)
{
	switch(status){
	case STATUS_ACTIVE:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
		break;
	case STATUS_ACTIVE_WARNING:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledYellowX0,ledYellowY0,ledWidht,ledHeight);
		break;
	case STATUS_ACTIVE_ERROR:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledRedX0,ledRedY0,ledWidht,ledHeight);
		break;
	case STATUS_PASSIVE:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
		break;
	case STATUS_PASSIVE_WARNING:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledYellowX0,ledYellowY0,ledWidht,ledHeight);
		break;
	case STATUS_PASSIVE_ERROR:
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledRedX0,ledRedY0,ledWidht,ledHeight);
		break;
	}

}

void StatusLight::drawActive (SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy,bool active)
{
	if(active)
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledGreenX0,ledGreenY0,ledWidht,ledHeight);
	else
		oapiBlt(tgt,src,xPos_+dx,yPos_+dy,ledBlackX0,ledBlackY0,ledWidht,ledHeight);
}
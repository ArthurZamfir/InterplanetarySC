#include "SubSystemInstrument.h"
#include "Logger.h"
#include <math.h>
#include <string>

/*
Es werden hier keine unterelemente im constructor initialisiert,
da durch eine doppelte iteration hier eine runtime-exception auftritt.
Stattdessen wird von Aussen nach Erstellung der Objekte die funktion
initialize() aufgerufen.
*/
SubSystemInstrument::SubSystemInstrument (VESSEL3 *v,SubSystem *subSystem,DWORD x,DWORD y)
  : PanelElement (v)
{
	ss = subSystem;
	xPos = x;
	yPos = y;
	firstDraw = true;

	sw = new OperationSwitch(xPos,yPos);
	led1 = new StatusLight(xPos,yPos);
	led2 = new StatusLight(xPos,yPos);
	attributeCount = ss->getAllAttributes().size();

	//for(int i = 0; i < attributeCount ; i++){
	//	std::string attribute = ss-
	//}

	//Copy map
	std::map<std::string,double*> temp = ss->getAllAttributes();

	//create all Bar-Objects
		for (std::map<std::string,double*>::iterator it = temp.begin();
		it != temp.end(); ++it)
	{
		std::string attribute = it->first;
		double *val = it->second;
		double maxValue = ss->getAllMaxAttributes()[attribute];
		bar.push_back(Bar(attribute,xPos,yPos,val,maxValue));
	}


	
	switchedState = ss->isActive();
	operationMode = ss->getOperationMode();

}

//The source-file should be instruments.dds
bool SubSystemInstrument::Redraw2D(SURFHANDLE tgt,SURFHANDLE src)
{
	//Diese Funktion sorgt dafür, dass nur einmal pro sekunde
	//aktualisiert wird.
	if(!(oapiGetSysTime()-0.03>lastRefreshTime))
	{
		return false;
	}
	lastRefreshTime = oapiGetSysTime();

	//Initial Draw with SubSystem-Canvas
	if(firstDraw)
	{
		firstDraw = false;
		for (int i = 0; i <= attributeCount; i++)
		{
			oapiBlt(tgt,src,xPos,yPos+i*backHeight,backX0,backY0,backWidth,backHeight);
		}
		//Draw SubSystem Name
		drawText(ss->getName(),tgt,src,55,20);
		//Draw "Active"
		drawText("Active",tgt,src,75,35);
		//Draw "Status"
		drawText("Status",tgt,src,145,35);
		//Draw Switch
		if(switchedState){
			sw->drawSwitchON(tgt,src,10,10);
		}else{
			sw->drawSwitchOFF(tgt,src,10,10);
		}
		//Draw Lights
		led2->drawStatus(tgt,src,130,35,operationMode);
		led1->drawActive(tgt,src,60,35,switchedState);
		//Draw Bars
		int i=0;
		for (std::vector<Bar>::iterator it = bar.begin(); it != bar.end(); ++it)
		{
			it->draw(tgt,src,5,67+i*50);
			i++;
		}		
	}
	//Draw Switch
	if(ss->isActive() != switchedState){
		switchedState = ss->isActive();
		if(switchedState){
			sw->drawSwitchON(tgt,src,10,10);
			
		}			
		else{
			sw->drawSwitchOFF(tgt,src,10,10);
		}
		led1->drawActive(tgt,src,60,35,switchedState);
	}
	//Draw StatusLights
	if(ss->getOperationMode() != operationMode)
	{
		operationMode = ss->getOperationMode();
		led2->drawStatus(tgt,src,130,35,operationMode);
	}
	//Draw Bars
	int i=0;
	for (std::vector<Bar>::iterator it = bar.begin(); it != bar.end(); ++it)
		{
			it->draw(tgt,src,5,67+i*50);
			i++;
		}		

	return false;

}bool SubSystemInstrument::ProcessMouse2D (int event, int mx, int my)
{

		if(switchedState){
			ss->deactivate();
		}
		else{
			ss->activate();
		}
		return true;

}

void SubSystemInstrument::drawText(std::string text,SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	int w,i,x;
	int len = text.length();
	for (w = i = 0; i < len; i++)
        w += font_width[text[i]];
	for (i = 0, x = xPos + dx; i < len; i++)
	{
		w = font_width[text[i]];
		oapiBlt(tgt, src, x, yPos + dy, font_xpos[text[i]]+fontXStart,
			fontYStart, w, fontH);
		x += w;
	}
}

int SubSystemInstrument::getPanelHeight()
{
	int height = 50 + attributeCount*50;
	return height;
}

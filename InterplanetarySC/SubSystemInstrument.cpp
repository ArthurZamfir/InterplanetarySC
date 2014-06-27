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
SubSystemInstrument::SubSystemInstrument (VESSEL3* v,SubSystem* subSystem,DWORD x,DWORD y)
  : PanelElement (v)
{
	ss_ = subSystem;
	xPos_ = x;
	yPos_ = y;
	firstDraw_ = true;

	sw_ = new OperationSwitch(xPos_,yPos_);
	led1_ = new StatusLight(xPos_,yPos_);
	led2_ = new StatusLight(xPos_,yPos_);
	attributeCount_ = ss_->getAllAttributes().size();

	//Copy map
	std::map<std::string,double*> temp = ss_->getAllAttributes();

	//create all Bar-Objects
		for (std::map<std::string,double*>::iterator it = temp.begin();
		it != temp.end(); ++it)
	{
		std::string attribute = it->first;
		double *val = it->second;
		double maxValue = ss_->getAllMaxAttributes()[attribute];
		bar_.push_back(Bar(attribute,xPos_,yPos_,val,maxValue));
	}

	switchedState_ = ss_->isActive();
	status_ = ss_->getStatus();
}

//The source-file should be instruments.dds
bool SubSystemInstrument::Redraw2D (SURFHANDLE tgt,SURFHANDLE src)
{
	//Diese Funktion sorgt dafür, dass nur einmal pro sekunde
	//aktualisiert wird.
	if(!(oapiGetSysTime()-0.03>lastRefreshTime_))
	{
		return false;
	}
	lastRefreshTime_ = oapiGetSysTime();

	//Initial Draw with SubSystem-Canvas
	if(firstDraw_)
	{
		firstDraw_ = false;
		for (int i = 0; i <= attributeCount_; i++)
		{
			oapiBlt(tgt,src,xPos_,yPos_+i*backHeight,backX0,backY0,backWidth,backHeight);
		}
		//Draw SubSystem Name
		drawText(ss_->getName(),tgt,src,55,20);
		//Draw "Active"
		drawText("Active",tgt,src,75,35);
		//Draw "Status"
		drawText("Status",tgt,src,145,35);
		//Draw Switch
		if(switchedState_){
			sw_->drawSwitchON(tgt,src,10,10);
		}else{
			sw_->drawSwitchOFF(tgt,src,10,10);
		}
		//Draw Lights
		led2_->drawStatus(tgt,src,130,35,status_);
		led1_->drawActive(tgt,src,60,35,switchedState_);
		//Draw Bars
		int i=0;
		for (std::vector<Bar>::iterator it = bar_.begin(); it != bar_.end(); ++it)
		{
			it->draw(tgt,src,5,67+i*50);
			i++;
		}		
	}
	//Draw Switch
	if(ss_->isActive() != switchedState_){
		switchedState_ = ss_->isActive();
		if(switchedState_){
			sw_->drawSwitchON(tgt,src,10,10);
			
		}			
		else{
			sw_->drawSwitchOFF(tgt,src,10,10);
		}
		led1_->drawActive(tgt,src,60,35,switchedState_);
	}
	//Draw StatusLights
	if(ss_->getStatus() != status_)
	{
		status_ = ss_->getStatus();
		led2_->drawStatus(tgt,src,130,35,status_);
	}
	//Draw Bars
	int i=0;
	for (std::vector<Bar>::iterator it = bar_.begin(); it != bar_.end(); ++it)
		{
			it->draw(tgt,src,5,67+i*50);
			i++;
		}		

	return false;

}bool SubSystemInstrument::ProcessMouse2D (int event, int mx, int my)
{

		if(switchedState_){
			ss_->deactivate();
		}
		else{
			ss_->activate();
		}
		return true;

}

void SubSystemInstrument::drawText (std::string text,SURFHANDLE tgt,SURFHANDLE src,DWORD dx,DWORD dy)
{
	int w,i,x;
	int len = text.length();
	for (w = i = 0; i < len; i++)
        w += font_width[text[i]];
	for (i = 0, x = xPos_ + dx; i < len; i++)
	{
		w = font_width[text[i]];
		oapiBlt(tgt, src, x, yPos_ + dy, font_xpos[text[i]]+fontXStart,
			fontYStart, w, fontH);
		x += w;
	}
}

int SubSystemInstrument::getPanelHeight()
{
	int height = 50 + attributeCount_*50;
	return height;
}

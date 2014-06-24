#include "SubSystemInstrument.h"
#include "Logger.h"
#include <math.h>
#include <string>

SubSystemInstrument::SubSystemInstrument (VESSEL3 *v,SubSystem *subSystem,DWORD x,DWORD y)
  : PanelElement (v)
{
	ss = subSystem;
	xPos = x;
	yPos = y;
	sw = new OperationSwitch();
	led = new StatusLight();
}

void SubSystemInstrument::initializeInstrument()
{

}


//The source-file should be instruments.dds
bool SubSystemInstrument::Redraw2D(SURFHANDLE tgt,SURFHANDLE src)
{
	//Diese Funktion sorgt dafür, dass nur einmal pro sekunde
	//aktualisiert wird.
	if(!(oapiGetSysTime()-1.0>lastRefreshTime))
	{
		return false;
	}

	lastRefreshTime = oapiGetSysTime();

	
	return false;

}bool SubSystemInstrument::ProcessMouse2D (int event, int mx, int my)
{
    return false;
}
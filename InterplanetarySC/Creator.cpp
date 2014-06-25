#define ORBITER_MODULE
#include "Bar.h"
#include <Windows.h>
#include <stdio.h>
#include "orbitersdk.h"
#include "OrbiterAPI.h"
#include "ScnEditorAPI.h"
#include "Logger.h"
#include "SubSystem.h"
#include "InterplanetarySC.h"
#include <map>
#include "SubSystemLib.h"
#include "Link.h"
#include "SubSystemInstrument.h"


class InterplanetarySC:public VESSEL3
{
public:
	InterplanetarySC(OBJHANDLE hObj,int fmodel);
	~InterplanetarySC();
	void clbkSetClassCaps(FILEHANDLE cfg);
	bool clbkLoadPanel2D(int id,PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	static SURFHANDLE panel2dtex;
	static SURFHANDLE instrumentTex;
	static double orbitalSpeed;
	std::vector<SubSystem*> subsys;
	std::vector<Link*> links;
	double simTime;
	double simTimeLast;
	double *simTimePtr;
	bool clbkPanelMouseEvent(int id,int event,int mx,int my,void *context);
	bool clbkPanelRedrawEvent(int id,int event,SURFHANDLE surf,void *context);
	std::vector<SubSystemInstrument*> subSysInstruments;
	void clbkMFDMode (int mfd, int mode);
	void clbkPreStep(double simt,double simdt,double mjd);	
private:	
	PROPELLANT_HANDLE mainTank;
	THRUSTER_HANDLE mainThr;
	MESHHANDLE hPanelMesh;
	void runSubSystemSimulationStep(std::vector<SubSystem*> subsys,std::vector<Link*> links);
	void DefineMainPanel(PANELHANDLE hPanel);
	void ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	void createSubSystems();
};

HINSTANCE g_hInst;
SURFHANDLE InterplanetarySC::panel2dtex = NULL;
SURFHANDLE InterplanetarySC::instrumentTex = NULL;
double InterplanetarySC::orbitalSpeed = 15000;
Logger systemLog("Spacecraft_Systemlog.txt");
Logger subSystemLog("SubSystems.txt");

InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel)
{
	hPanelMesh = NULL;
	simTimePtr = &simTime;
	simTimeLast = 0;
	createSubSystems();
}

InterplanetarySC::~InterplanetarySC()
{
	if(hPanelMesh) oapiDeleteMesh(hPanelMesh);
	oapiDestroySurface(panel2dtex);
	oapiDestroySurface(instrumentTex);
	delete simTimePtr;
}

DLLCLBK void InitModule (HINSTANCE hModule)
{
	g_hInst = hModule;
	InterplanetarySC::panel2dtex = oapiLoadTexture("InterplanetarySC\\blank.dds");
	InterplanetarySC::instrumentTex = oapiLoadTexture("InterplanetarySC\\instruments.dds");
	systemLog.logLine("Initialisiert");

}

void InterplanetarySC::createSubSystems()
{
	WaterTank *water1 = new WaterTank(this,"WasserTank1",simTimePtr,4000,5000);
	OxygenTank *oxy1 = new OxygenTank(this,"SauerstoffTank1",simTimePtr,5000,5000);
	HydrogenTank *hydro1 = new HydrogenTank(this,"WasserstoffTank1",simTimePtr,5000,5000);
	Thruster *thrus1 = new Thruster(this,"Thruster1",simTimePtr,8000,4000,"MAIN");
	FuelCell *fc1 = new FuelCell(this,"Brennstoffzelle1",simTimePtr,0.4,4,1000);
	Battery *bat1 = new Battery(this,"Batterie1",simTimePtr,100,1000);
	Radiator *radiator1 = new Radiator(this,"Radiator1",simTimePtr,0,50000);
	/*Thruster *thrus2 = new Thruster("Thruster2");
	Thruster *thrus3 = new Thruster("Thruster3");
	Thruster *thrus4 = new Thruster("Thruster4");*/
	
	/*Battery *bat2 = new Battery("Batterie2");
	Battery *bat3 = new Battery("Batterie3");*/
	//ThermalFissionGenerator *tfg1 = new ThermalFissionGenerator("Reaktor1");
	

	subsys.push_back(water1);
	subsys.push_back(oxy1);
	subsys.push_back(hydro1);
	subsys.push_back(thrus1);
	/*subsys.push_back(thrus2);
	subsys.push_back(thrus3);
	subsys.push_back(thrus4);*/
	subsys.push_back(fc1);
	subsys.push_back(bat1);
	//subsys.push_back(bat2);
	//subsys.push_back(bat3);
	//subsys.push_back(tfg1);
	subsys.push_back(radiator1);

	//Links instanzieren
	Link *h2a = new Link("H2");
	Link *h2b = new Link("H2");
	Link *o2a = new Link("O2");
	Link *o2b = new Link("O2");
	Link *h2oa = new Link("H2O");
	Link *h2ob = new Link("H2O");
	Link *heat1 = new Link("Heat[J]");
	Link *power1 = new Link("Energy[J]");

	links.push_back(h2a);
	links.push_back(h2b);
	links.push_back(o2a);
	links.push_back(o2b);
	links.push_back(h2oa);
	links.push_back(h2ob);
	links.push_back(heat1);
	links.push_back(power1);

	//Ports verbinden
	hydro1->connectPortToOutput(h2a->getPort());
	hydro1->connectPortToOutput(h2b->getPort());
	oxy1->connectPortToOutput(o2a->getPort());
	oxy1->connectPortToOutput(o2b->getPort());
	water1->connectPortToOutput(h2oa->getPort());
	water1->connectPortToInput(h2ob->getPort());
	thrus1->connectPortToInput(h2a->getPort());
	thrus1->connectPortToInput(o2a->getPort());
	fc1->connectPortToInput(h2b->getPort());
	fc1->connectPortToInput(o2b->getPort());
	fc1->connectPortToInput(h2oa->getPort());
	fc1->connectPortToOutput(h2ob->getPort());
	fc1->connectPortToOutput(power1->getPort());
	fc1->connectPortToOutput(heat1->getPort());
	radiator1->connectPortToInput(heat1->getPort());
	bat1->connectPortToInput(power1->getPort());
}


DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	systemLog.logLine("Exit");
}

void InterplanetarySC::clbkPreStep(double simt,double simdt,double mjd)
{
	simTime = simdt;
	runSubSystemSimulationStep(subsys,links);
	orbitalSpeed = orbitalSpeed - simt;
}

void InterplanetarySC::runSubSystemSimulationStep(std::vector<SubSystem*> subsys,std::vector<Link*> links)
{
	std::vector<SubSystem*>::iterator ssit;
	std::vector<Link*>::iterator linkit;

		for (ssit = subsys.begin();ssit!=subsys.end();++ssit)
		{
			(*ssit)->calculateStep();
		}

		for (linkit = links.begin();linkit!=links.end();++linkit)
		{
			(*linkit)->transfer();
		}

		for (ssit = subsys.begin();ssit!=subsys.end();++ssit)
	{
		subSystemLog.logLine((*ssit)->report());
	}
}
bool InterplanetarySC::clbkLoadPanel2D(int id,PANELHANDLE hPanel,
									   DWORD viewW, DWORD viewH)
{
	systemLog.logLine("Load2DPanel");
	
	switch(id)
	{
	case 0:
		DefineMainPanel(hPanel);
		ScalePanel(hPanel, viewW,viewH);
		oapiSetHUDMode(HUD_NONE);
		return true;
	default:
		oapiSetHUDMode(HUD_ORBIT);
		return false;
	}

}

void InterplanetarySC::DefineMainPanel(PANELHANDLE hPanel)
{
	systemLog.logLine("DefineMainPanel");
	static DWORD panelW = 2048;
	static DWORD panelH = 1024;
	float fpanelW = (float)panelW;
	float fpanbelH = (float)panelH;
	static DWORD texW = 2048;
	static DWORD texH = 1024;
	float ftexW = (float)texW;
	float ftexH = (float)texH;

	static NTVERTEX VTX[4] = {
		{0,0,0,0,0,0,0.0f,1.0f-fpanbelH/ftexH},
		{0,fpanbelH,0,0,0,0,0.0f,1.0f},
		{fpanelW,fpanbelH,0,0,0,0,fpanelW/ftexW,1.0f},
		{fpanelW,0,0,0,0,0,fpanelW/ftexW,1.0f-fpanbelH/ftexH},
	};
	static WORD IDX[6] = {
		0,2,1,
		2,0,3
	};

	if(hPanelMesh) oapiDeleteMesh(hPanelMesh);
	hPanelMesh = oapiCreateMesh(0,0);
	MESHGROUP grp = {VTX,IDX,4,6,0,0,0,0,0};
	oapiAddMeshGroup(hPanelMesh,&grp);
	systemLog.logLine("SetPanelBack");
	SetPanelBackground(hPanel,&panel2dtex,1,hPanelMesh,panelW,panelH,0,
		PANEL_ATTACH_BOTTOM | PANEL_MOVEOUT_BOTTOM);

  //Bar Test
	int gap = 30;
	int yCurrent = 30;
	int xCurrent = 30;
	int ySpaceLeft = panelH - yCurrent;
	int areaID = 1000;
	std::vector<SubSystem*>::iterator it;
	for (it = subsys.begin(); it != subsys.end(); ++it)
   {
	   //Abfrage ob noch genug Abstand in der aktuellen Spalte vorhanden ist.
	   //Wenn nicht wird in der nächsten Spalte weiter gemacht.
	   int instrumentHeight = 50 + (*it)->getAllAttributes().size()*50;
	   if((ySpaceLeft - instrumentHeight) < 30){
		   xCurrent = xCurrent + 230;
		   yCurrent = 30;
		   ySpaceLeft = panelH - yCurrent;
	   }else{
		   ySpaceLeft = ySpaceLeft - instrumentHeight -gap;
		   yCurrent = yCurrent + instrumentHeight +gap;
	   }
		subSysInstruments.push_back(new SubSystemInstrument(this,(*it),xCurrent,yCurrent));
		RegisterPanelArea(hPanel, areaID, _R(xCurrent+10,yCurrent+10,xCurrent+40,yCurrent+40),
		PANEL_REDRAW_ALWAYS,
		PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY,
		panel2dtex, subSysInstruments.back());
		areaID++;
   }

}



void InterplanetarySC::ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH)
{
	systemLog.logLine("ScalePanel");
	double defscale = (double)viewW/2048;
	double magscale = max (defscale,1.0);
	SetPanelScaling(hPanel,defscale,magscale);
}

bool InterplanetarySC::clbkPanelMouseEvent(int id,int event,int mx,int my,void *context)
{
	std::string s = std::to_string(event)  +" - " + std::to_string(mx)  +" - "+ std::to_string(my);
  if (context) {
    PanelElement *pe = (PanelElement*)context;
    return pe->ProcessMouse2D (event, mx, my);
  } else
    return false;

}

bool InterplanetarySC::clbkPanelRedrawEvent(int id,int event,SURFHANDLE surf,void *context)
{
	systemLog.logLine("Redraw without context!");
  if (context) {
	  systemLog.logLine("Redraw");
    PanelElement *pe = (PanelElement*)context;
	return pe->Redraw2D (surf,instrumentTex);
	//return pe->Redraw2D (fontTex);
  } else
    return false;

	//if(oapiGetSysTime()-1>time)
	//{
	//	time = oapiGetSysTime();
	//	PanelElement *pe = (PanelElement*)context;
	//	return pe->Redraw2D (surf,instrumentTex);
	//}
	//return false;

}

void InterplanetarySC::clbkMFDMode (int mfd, int mode)
{
  switch (mfd) {
  case MFD_LEFT:
	  systemLog.logLine("MFD_DRAW");
    oapiTriggerRedrawArea (0, 0, AID_MFD_LBUTTONS);
    oapiTriggerRedrawArea (0, 0, AID_MFD_RBUTTONS);
    break;
  }
}
 
void InterplanetarySC::clbkSetClassCaps(FILEHANDLE cfg)
{
	MESHHANDLE mesh;
	mesh = oapiLoadMeshGlobal("ThermalFissionCraft"); 
	
	UINT idx;
	idx = AddMesh(mesh);

	SetMeshVisibilityMode(idx,MESHVIS_EXTERNAL);

	
	SetSize(350.0); 
	SetEmptyMass(5000.0);

	Logger l("LOGtest.txt");
	l.logLine("Das ist eine Zeile");
	l.logLine("Das ist eine andere Zeile");
	l.logLine("Die meisten Menschen sind verwirrt, wenn Sätze nicht so aufhören wie sie Banane.");
	/*SubSystem testSys("Brennstoffzelle");
	testSys.addAttribute("H2O",100.0);
	testSys.addAttribute("H2",200.0);
	testSys.addAttribute("O2",300.0);*/
	//l.logLine(testSys.report());

	mainTank = CreatePropellantResource(8000.0);
	mainThr = CreateThruster(_V(0.0,0.0,-290.0),_V(0.0,0.0,1.0),800000.0,mainTank,8000.0);
	THRUSTER_HANDLE group1[1];
	group1[0] = mainThr;
	CreateThrusterGroup(group1,1,THGROUP_ATT_FORWARD);
	AddExhaust(mainThr,1000,10); 

}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel,int flightmodel)
{
	VESSEL3* pVessel;
	pVessel = new InterplanetarySC(hvessel,flightmodel);
	return pVessel;
}
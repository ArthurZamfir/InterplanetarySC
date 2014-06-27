#define ORBITER_MODULE
#include "Bar.h"
#include <Windows.h>
#include <stdio.h>
#include "orbitersdk.h"
#include "OrbiterAPI.h"
#include "ScnEditorAPI.h"
#include "Logger.h"
#include "SubSystem.h"
#include "Thruster.h"
#include "InterplanetarySC.h"
#include <map>
#include "SubSystemLib.h"
#include "Link.h"
#include "SubSystemInstrument.h"
#include "TexturePositions.h"


class InterplanetarySC:public VESSEL3
{
public:

	

	InterplanetarySC(OBJHANDLE hObj,int fmodel);
	~InterplanetarySC();
	void clbkSetClassCaps(FILEHANDLE cfg);
	bool clbkLoadPanel2D(int id,PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	bool clbkPanelMouseEvent(int id,int event,int mx,int my,void *context);
	bool clbkPanelRedrawEvent(int id,int event,SURFHANDLE surf,void *context);	
	void clbkPreStep(double simt,double simdt,double mjd);
private:	
  	static SURFHANDLE panel2dtex_;
	static SURFHANDLE instrumentTex_;
	static double orbitalSpeed_;
	std::vector<SubSystem*> subsys_;
	std::vector<Link*> links_;
	double simTime_;
	double simTimeLast_;
	double* simTimePtr_;
	std::vector<SubSystemInstrument*> subSysInstruments_;
	PROPELLANT_HANDLE mainTank_;
	MESHHANDLE hPanelMesh_;

	void runSubSystemSimulationStep(std::vector<SubSystem*> subsys,std::vector<Link*> links);
	void DefineMainPanel(PANELHANDLE hPanel);
	void ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	void createSubSystems();
};

SURFHANDLE InterplanetarySC::panel2dtex_ = NULL;
SURFHANDLE InterplanetarySC::instrumentTex_ = NULL;
double InterplanetarySC::orbitalSpeed_ = 15000;
Logger systemLog("Spacecraft_Systemlog.txt");
Logger subSystemLog("SubSystems.txt");

InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel)
{
	hPanelMesh_ = NULL;
	simTimePtr_ = &simTime_;
	simTimeLast_ = 0;
	InterplanetarySC::panel2dtex_ = oapiLoadTexture("InterplanetarySC\\blank.dds");
	InterplanetarySC::instrumentTex_ = oapiLoadTexture("InterplanetarySC\\instruments.dds");
}

InterplanetarySC::~InterplanetarySC()
{
	if(hPanelMesh_) oapiDeleteMesh(hPanelMesh_);
	oapiDestroySurface(panel2dtex_);
	oapiDestroySurface(instrumentTex_);
	delete simTimePtr_;
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel,int flightmodel)
{
	VESSEL3* pVessel;
	pVessel = new InterplanetarySC(hvessel,flightmodel);
	return pVessel;
}

void InterplanetarySC::clbkSetClassCaps(FILEHANDLE cfg)
{
	MESHHANDLE mesh;
	mesh = oapiLoadMeshGlobal("ThermalFissionCraft"); 
	
	UINT idx;
	idx = AddMesh(mesh);

	SetMeshVisibilityMode(idx,MESHVIS_EXTERNAL);
		
	SetSize(350.0); 
	SetEmptyMass(400000.0);
	createSubSystems();

}

void InterplanetarySC::createSubSystems()
{
	/*
	Erstellung der Treibstofftanks.
	Es wurde hier bewusst darauf verzichtet die Treibstofftanks direkt in den
	Subsystemen zu erstellen, da Orbiter ohnehin nicht zwischen propellant und oxidant
	unterscheidet. Dennoch wird mitgerechnet mit einer stöchiometrischen Verbrennung,
	das heißt dass es je 2 Tanks mit propellant und oxidant gibt, einmal für das
	Haupttriebwerk und einmal für die Raktionsdüsen.
	*/
	double mainPropMass = 800000.0;
	double rcsPropMass = 6000.0;
	double stoechioMainHydro = mainPropMass * 2/10;
	double stoechioMainOxy = mainPropMass * 8/10;
	double stoechioRCSHydro = rcsPropMass * 2/10;
	double stoechioRCSOxy = rcsPropMass * 8/10;
	PROPELLANT_HANDLE orbiterMainTank = CreatePropellantResource(mainPropMass);
	PROPELLANT_HANDLE orbiterRCSTank = CreatePropellantResource(rcsPropMass);

	HydrogenTank *hydroMain = new HydrogenTank(this,"Wasserstoff Haupt",simTimePtr_,stoechioMainHydro,stoechioMainHydro);
	OxygenTank *oxyMain = new OxygenTank(this,"Sauerstoff Haupt",simTimePtr_,stoechioMainOxy,stoechioMainOxy);
	HydrogenTank *hydroRCS = new HydrogenTank(this,"WasserstoffTank1",simTimePtr_,stoechioMainHydro,stoechioMainHydro);
	OxygenTank *oxyRCS = new OxygenTank(this,"SauerstoffTank1",simTimePtr_,stoechioMainOxy,stoechioMainOxy);

	/*
	Erstellung der Main Engine
	*/
	THGROUP_TYPE mainEngineType[] = {THGROUP_ATT_FORWARD};
	Thruster *mainEngine = new Thruster(this,"Main Engine",simTimePtr_,_V(0,0,-290),_V(0,0,1),350000,orbiterMainTank,8300,mainEngineType,1,200,10);
	/*
	Erstellung der RCS Thruster
	*/
	double rcsTh = 50;
	double rcsTh1 = 150;
	double rcsIsp = 20000;
	double rcsL = 20;
	double rcsW = 1;
	THGROUP_TYPE rcsGroup1[] = {THGROUP_ATT_PITCHUP};
	THGROUP_TYPE rcsGroup2[] = {THGROUP_ATT_YAWRIGHT,THGROUP_ATT_BANKLEFT};
	THGROUP_TYPE rcsGroup3[] = {THGROUP_ATT_YAWLEFT,THGROUP_ATT_BANKRIGHT};
	THGROUP_TYPE rcsGroup4[] = {THGROUP_ATT_YAWLEFT,THGROUP_ATT_PITCHDOWN};
	THGROUP_TYPE rcsGroup5[] = {THGROUP_ATT_YAWLEFT,THGROUP_ATT_PITCHUP,THGROUP_ATT_BANKLEFT};
	THGROUP_TYPE rcsGroup6[] = {THGROUP_ATT_PITCHDOWN,THGROUP_ATT_BANKRIGHT};
	THGROUP_TYPE rcsGroup7[] = {THGROUP_ATT_YAWRIGHT,THGROUP_ATT_PITCHDOWN};
	THGROUP_TYPE rcsGroup8[] = {THGROUP_ATT_YAWRIGHT,THGROUP_ATT_PITCHUP,THGROUP_ATT_BANKRIGHT};
	THGROUP_TYPE rcsGroup9[] = {THGROUP_ATT_PITCHDOWN,THGROUP_ATT_BANKLEFT};
	THGROUP_TYPE rcsGroup10[] = {THGROUP_ATT_PITCHUP};
	THGROUP_TYPE rcsGroup11[] = {THGROUP_ATT_YAWRIGHT,THGROUP_ATT_PITCHUP};
	THGROUP_TYPE rcsGroup12[] = {THGROUP_ATT_YAWLEFT,THGROUP_ATT_PITCHUP};
	THGROUP_TYPE rcsGroup13[] = {THGROUP_ATT_FORWARD};
	THGROUP_TYPE rcsGroup14[] = {THGROUP_ATT_BACK};
	THGROUP_TYPE rcsGroup15[] = {THGROUP_ATT_FORWARD};
	THGROUP_TYPE rcsGroup16[] = {THGROUP_ATT_BACK};
	THGROUP_TYPE rcsGroup17[] = {THGROUP_ATT_FORWARD};
	THGROUP_TYPE rcsGroup18[] = {THGROUP_ATT_BACK};
	
	Thruster *rcs1 = new Thruster(this,"RCS 1",simTimePtr_,_V(0,-30.9,52.9),_V(0,1,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup1,1,rcsL,rcsW);
	Thruster *rcs2 = new Thruster(this,"RCS 2",simTimePtr_,_V(-0.5,-30.5,52.9),_V(1,0,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup2,2,rcsL,rcsW);
	Thruster *rcs3 = new Thruster(this,"RCS 3",simTimePtr_,_V(0.5,-30.5,52.9),_V(-1,0,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup3,2,rcsL,rcsW);
	Thruster *rcs4 = new Thruster(this,"RCS 4",simTimePtr_,_V(26.7,15.4,52.9),_V(-0.86,-0.5,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup4,2,rcsL,rcsW);
	Thruster *rcs5 = new Thruster(this,"RCS 5",simTimePtr_,_V(26.7,14.8,52.9),_V(-0.5,0.86,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup5,3,rcsL,rcsW);
	Thruster *rcs6 = new Thruster(this,"RCS 6",simTimePtr_,_V(26.1,15.7,52.9),_V(0.5,-0.86,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup6,2,rcsL,rcsW);
	Thruster *rcs7 = new Thruster(this,"RCS 7",simTimePtr_,_V(-26.7,15.4,52.9),_V(0.86,-0.5,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup7,2,rcsL,rcsW);
	Thruster *rcs8 = new Thruster(this,"RCS 8",simTimePtr_,_V(-26.7,14.8,52.9),_V(0.5,0.86,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup8,3,rcsL,rcsW);
	Thruster *rcs9 = new Thruster(this,"RCS 9",simTimePtr_,_V(-26.1,15.7,52.9),_V(-0.5,-0.86,0),rcsTh,orbiterRCSTank,rcsIsp,rcsGroup9,2,rcsL,rcsW);
	Thruster *rcs10 = new Thruster(this,"RCS 10",simTimePtr_,_V(0,-1.5,-232.7),_V(0,-1,0),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup10,1,rcsL,rcsW);
	Thruster *rcs11 = new Thruster(this,"RCS 11",simTimePtr_,_V(1.3,0.7,-232.6),_V(-0.86,-0.5,0),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup11,2,rcsL,rcsW);
	Thruster *rcs12 = new Thruster(this,"RCS 12",simTimePtr_,_V(-1.3,0.7,-232.6),_V(0.86,-0.5,0),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup12,2,rcsL,rcsW);
	Thruster *rcs13 = new Thruster(this,"RCS 13",simTimePtr_,_V(0,-30.5,52.3),_V(0,0,1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup13,1,rcsL,rcsW);
	Thruster *rcs14 = new Thruster(this,"RCS 14",simTimePtr_,_V(0,-30.5,53.4),_V(0,0,-1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup14,1,rcsL,rcsW);
	Thruster *rcs15 = new Thruster(this,"RCS 15",simTimePtr_,_V(26.4,15.3,52.3),_V(0,0,1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup15,1,rcsL,rcsW);
	Thruster *rcs16 = new Thruster(this,"RCS 16",simTimePtr_,_V(26.4,15.3,53.4),_V(0,0,-1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup16,1,rcsL,rcsW);
	Thruster *rcs17 = new Thruster(this,"RCS 17",simTimePtr_,_V(-26.4,15.3,52.3),_V(0,0,1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup17,1,rcsL,rcsW);
	Thruster *rcs18 = new Thruster(this,"RCS 18",simTimePtr_,_V(-26.4,15.3,53.4),_V(0,0,-1),rcsTh1,orbiterRCSTank,rcsIsp,rcsGroup18,1,rcsL,rcsW);

	WaterTank *water1 = new WaterTank(this,"WasserTank1",simTimePtr_,4000,50000);
	
	

	FuelCell *fc1 = new FuelCell(this,"Brennstoffzelle1",simTimePtr_,0.4,4,3000);
	fc1->deactivate();
	hydroMain->setStatus(STATUS_ACTIVE_WARNING);
	//Battery *bat1 = new Battery(this,"Batterie1",simTimePtr,100,1000,0.64,0.64,300,270,340);
	//Battery *bat1 = new Battery(this,"Batterie1",simTimePtr,100,1000,10,10,0.64,0.64,300,270,340,5);
	Battery *bat1 = new Battery(this,"Batterie1",simTimePtr_,50000,900000,1000,10000,0.9,0.9,300,270,320,5000,400,5);
	Radiator *radiator1 = new Radiator(this,"Radiator1",simTimePtr_,5000,1000000,100);
	Heater *heater1 = new Heater(this,"Heater1",simTimePtr_,5000000,0.8,0.8);
	/*Thruster *thrus2 = new Thruster("Thruster2");
	Thruster *thrus3 = new Thruster("Thruster3");
	Thruster *thrus4 = new Thruster("Thruster4");*/
	
	/*Battery *bat2 = new Battery("Batterie2");
	Battery *bat3 = new Battery("Batterie3");*/
	//ThermalFissionGenerator *tfg1 = new ThermalFissionGenerator("Reaktor1");
	//SubSystemInstrument(this,water1,5,5);
	//subSysInstruments.push_back(new SubSystemInstrument(this,water1,5,5));

	subsys_.push_back(water1);
	subsys_.push_back(oxyMain);
	subsys_.push_back(hydroMain);
	subsys_.push_back(oxyRCS);
	subsys_.push_back(hydroRCS);
	subsys_.push_back(mainEngine);
	subsys_.push_back(fc1);
	subsys_.push_back(bat1);
	subsys_.push_back(radiator1);
	subsys_.push_back(heater1);

	//subsys_.push_back(rcs1);
	//subsys_.push_back(rcs2);
	//subsys_.push_back(rcs3);
	//subsys_.push_back(rcs4);
	//subsys_.push_back(rcs5);
	//subsys_.push_back(rcs6);
	//subsys_.push_back(rcs7);
	//subsys_.push_back(rcs8);
	//subsys_.push_back(rcs9);
	//subsys_.push_back(rcs10);
	//subsys_.push_back(rcs11);
	//subsys_.push_back(rcs12);
	//subsys_.push_back(rcs13);
	//subsys_.push_back(rcs14);
	//subsys_.push_back(rcs15);
	//subsys_.push_back(rcs16);
	//subsys_.push_back(rcs17);
	//subsys_.push_back(rcs18);



	//subsys_.push_back(thrus2);
	//subsys_.push_back(thrus3);
	/*subsys_.push_back(thrus4);*/

	//subsys_.push_back(bat2);
	//subsys_.push_back(bat3);
	//subsys_.push_back(tfg1);


	//Links instanzieren
	Link *h2a = new Link("H2");
	Link *h2b = new Link("H2");
	Link *o2a = new Link("O2");
	Link *o2b = new Link("O2");
	Link *h2oa = new Link("H2O");
	Link *h2ob = new Link("H2O");
	Link *power1 = new Link("Energy[J]");
	Link *power2 = new Link("Energy[J]");
	Link *heat1 = new Link("Heat[J]");
	Link *heat2 = new Link("Heat[J]");
	Link *heat3 = new Link("Heat[J]");


	links_.push_back(h2a);
	links_.push_back(h2b);
	links_.push_back(o2a);
	links_.push_back(o2b);
	links_.push_back(h2oa);
	links_.push_back(h2ob);
	links_.push_back(heat1);
	links_.push_back(power1);
	links_.push_back(power2);
	links_.push_back(heat2);
	links_.push_back(heat3);

	//Ports verbinden
	hydroMain->connectPortToOutput(h2a->getPort());
	hydroMain->connectPortToOutput(h2b->getPort());
	oxyMain->connectPortToOutput(o2a->getPort());
	oxyMain->connectPortToOutput(o2b->getPort());
	water1->connectPortToOutput(h2oa->getPort());
	water1->connectPortToInput(h2ob->getPort());
	mainEngine->connectPortToInput(h2a->getPort());
	mainEngine->connectPortToInput(o2a->getPort());
	//rcs1->connectPortToInput(h2a->getPort());
	//rcs1->connectPortToInput(o2a->getPort());
	//rcs2->connectPortToInput(h2a->getPort());
	//rcs2->connectPortToInput(o2a->getPort());
	//rcs3->connectPortToInput(h2a->getPort());
	//rcs3->connectPortToInput(o2a->getPort());
	//rcs4->connectPortToInput(h2a->getPort());
	//rcs4->connectPortToInput(o2a->getPort());
	//rcs5->connectPortToInput(h2a->getPort());
	//rcs5->connectPortToInput(o2a->getPort());
	//rcs6->connectPortToInput(h2a->getPort());
	//rcs6->connectPortToInput(o2a->getPort());
	//rcs7->connectPortToInput(h2a->getPort());
	//rcs7->connectPortToInput(o2a->getPort());
	//rcs8->connectPortToInput(h2a->getPort());
	//rcs8->connectPortToInput(o2a->getPort());
	//rcs9->connectPortToInput(h2a->getPort());
	//rcs9->connectPortToInput(o2a->getPort());
	//rcs10->connectPortToInput(h2a->getPort());
	//rcs10->connectPortToInput(o2a->getPort());
	//rcs11->connectPortToInput(h2a->getPort());
	//rcs11->connectPortToInput(o2a->getPort());
	//rcs12->connectPortToInput(h2a->getPort());
	//rcs12->connectPortToInput(o2a->getPort());
	//rcs13->connectPortToInput(h2a->getPort());
	//rcs13->connectPortToInput(o2a->getPort());
	//rcs14->connectPortToInput(h2a->getPort());
	//rcs14->connectPortToInput(o2a->getPort());
	//rcs15->connectPortToInput(h2a->getPort());
	//rcs15->connectPortToInput(o2a->getPort());
	//rcs16->connectPortToInput(h2a->getPort());
	//rcs16->connectPortToInput(o2a->getPort());
	//rcs17->connectPortToInput(h2a->getPort());
	//rcs17->connectPortToInput(o2a->getPort());
	//rcs18->connectPortToInput(h2a->getPort());
	//rcs18->connectPortToInput(o2a->getPort());

	//thrus2->connectPortToInput(h2a->getPort());
	//thrus2->connectPortToInput(o2a->getPort());
	//thrus3->connectPortToInput(h2a->getPort());
	//thrus3->connectPortToInput(o2a->getPort());
	fc1->connectPortToInput(h2b->getPort());
	fc1->connectPortToInput(o2b->getPort());
	fc1->connectPortToInput(h2oa->getPort());
	fc1->connectPortToOutput(h2ob->getPort());
	fc1->connectPortToOutput(power1->getPort());
	fc1->connectPortToOutput(heat1->getPort());
	fc1->connectPortToOutput(power2->getPort());

	bat1->connectPortToInput(power1->getPort());
	bat1->connectPortToInput(heat2->getPort());
	bat1->connectPortToOutput(heat3->getPort());
	heater1->connectPortToInput(power2->getPort());
	heater1->connectPortToOutput(heat2->getPort());
	radiator1->connectPortToInput(heat1->getPort());
	radiator1->connectPortToInput(heat3->getPort());

}


DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	systemLog.logLine("Exit");
}

void InterplanetarySC::clbkPreStep(double simt,double simdt,double mjd)
{
	simTime_ = simdt;
	runSubSystemSimulationStep(subsys_,links_);
	orbitalSpeed_ = orbitalSpeed_ - simt;
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

	//	for (ssit = subsys.begin();ssit!=subsys.end();++ssit)
	//{
	//	subSystemLog.logLine((*ssit)->report());
	//}
}
bool InterplanetarySC::clbkLoadPanel2D(int id,PANELHANDLE hPanel,
									   DWORD viewW, DWORD viewH)
{
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
	static DWORD panelW = 4096;
	static DWORD panelH = 2048;
	float fpanelW = (float)panelW;
	float fpanbelH = (float)panelH;
	static DWORD texW = 4096;
	static DWORD texH = 2048;
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

	if(hPanelMesh_) oapiDeleteMesh(hPanelMesh_);
	hPanelMesh_ = oapiCreateMesh(0,0);
	MESHGROUP grp = {VTX,IDX,4,6,0,0,0,0,0};
	oapiAddMeshGroup(hPanelMesh_,&grp);
	SetPanelBackground(hPanel,&panel2dtex_,1,hPanelMesh_,panelW,panelH,0,
		PANEL_ATTACH_BOTTOM | PANEL_MOVEOUT_BOTTOM);

  //Bar Test
	int gap = 50;
	int yCurrent = 50;
	int xCurrent = 50;
	int ySpaceLeft = panelH - yCurrent;
	int areaID = 1000;
	std::vector<SubSystem*>::iterator it;
	for (it = subsys_.begin(); it != subsys_.end(); ++it){
	   //Abfrage ob noch genug Abstand in der aktuellen Spalte vorhanden ist.
	   //Wenn nicht wird in der nächsten Spalte weiter gemacht.
	   int instHeight = 50 + (*it)->getAllAttributes().size()*50;
	   if((ySpaceLeft - instHeight) < gap){
		   xCurrent = xCurrent + backWidth + gap;
		   yCurrent = gap;
		   ySpaceLeft = panelH - yCurrent;
	   }
		subSysInstruments_.push_back(new SubSystemInstrument(this,(*it),xCurrent,yCurrent));
		RegisterPanelArea(hPanel, areaID, _R(xCurrent+10,yCurrent+10,xCurrent+40,yCurrent+40),
		PANEL_REDRAW_ALWAYS,
		PANEL_MOUSE_LBDOWN,
		panel2dtex_, subSysInstruments_.back());
		areaID++;
		
		ySpaceLeft = ySpaceLeft - instHeight -gap;
		yCurrent = yCurrent + instHeight +gap;
    }
	//Es wird hier nochmals über alle Subsyteme iteriert, um diese zu initialisieren.
	//Eine initialisierung in der obigen Schleife verursacht durch einen doppelten
	//Iteratorzugriff eine runtime-exception
	//std::vector<SubSystemInstrument*>::iterator iti;
	//for(iti = subSysInstruments.begin();iti != subSysInstruments.end(); ++iti){
	//	(*iti)->initialize();
	//}


}



void InterplanetarySC::ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH)
{
	double defscale = (double)viewW/4096;
	double magscale = max (defscale,1.0);
	SetPanelScaling(hPanel,defscale,magscale);
}

bool InterplanetarySC::clbkPanelMouseEvent(int id,int event,int mx,int my,void *context)
{
  if (context) {
    PanelElement *pe = (PanelElement*)context;
    return pe->ProcessMouse2D (event, mx, my);
  } else
    return false;

}

bool InterplanetarySC::clbkPanelRedrawEvent(int id,int event,SURFHANDLE surf,void *context)
{
  if (context) {
    PanelElement *pe = (PanelElement*)context;
	return pe->Redraw2D (surf,instrumentTex_);
  } else
    return false;

}


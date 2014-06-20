#define ORBITER_MODULE
#include "resource.h"
#include <Windows.h>
#include <stdio.h>
#include "orbitersdk.h"
#include "OrbiterAPI.h"
#include "ScnEditorAPI.h"
#include "Logger.h"
#include "SubSystem.h"


HINSTANCE g_hInst;



class InterplanetarySC:public VESSEL3
{
public:
	
	InterplanetarySC(OBJHANDLE hObj,int fmodel);
	~InterplanetarySC();
	void clbkSetClassCaps(FILEHANDLE cfg);
	bool clbkLoadPanel2D(int id,PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	static SURFHANDLE panel2dtex;
private:
	
	PROPELLANT_HANDLE mainTank;
	THRUSTER_HANDLE mainThr;
	MESHHANDLE hPanelMesh;
	double *a;
	void DefineMainPanel(PANELHANDLE hPanel);
	void ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
};

SURFHANDLE InterplanetarySC::panel2dtex = NULL;

InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel)
{
	hPanelMesh = NULL;
}

DLLCLBK void InitModule (HINSTANCE hModule)
{
	g_hInst = hModule;
	InterplanetarySC::panel2dtex = oapiLoadTexture("InterplanetarySC\\panel2d.dds");
	
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
}



InterplanetarySC::~InterplanetarySC()
{
	if(hPanelMesh) oapiDeleteMesh(hPanelMesh);
	oapiDestroySurface(panel2dtex);
}


bool InterplanetarySC::clbkLoadPanel2D(int id,PANELHANDLE hPanel,
									   DWORD viewW, DWORD viewH)
{
	switch(id)
	{
	case 0:
		DefineMainPanel(hPanel);
		ScalePanel(hPanel, viewW,viewH);
		return true;
	default:
		return false;
	}

}

void InterplanetarySC::DefineMainPanel(PANELHANDLE hPanel)
{
	static DWORD panelW = 1280;
	static DWORD panelH = 400;
	float fpanelW = (float)panelW;
	float fpanbelH = (float)panelH;
	static DWORD texW = 2048;
	static DWORD texH = 512;
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
	SetPanelBackground(hPanel,&panel2dtex,1,hPanelMesh,panelW,panelH,0,
		PANEL_ATTACH_BOTTOM | PANEL_MOVEOUT_BOTTOM);
}



void InterplanetarySC::ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH)
{
	double defscale = (double)viewW/1280.0;
	double magscale = max (defscale,1.0);
	SetPanelScaling(hPanel,defscale,magscale);
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
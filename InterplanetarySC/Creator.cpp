#define ORBITER_MODULE
#include "resource.h"
#include "Switch.h"
#include <Windows.h>
#include <stdio.h>
#include "orbitersdk.h"
#include "OrbiterAPI.h"
#include "ScnEditorAPI.h"
#include "Logger.h"
#include "SubSystem.h"
#include "InterplanetarySC.h"


class InterplanetarySC:public VESSEL3
{
public:
	InterplanetarySC(OBJHANDLE hObj,int fmodel);
	~InterplanetarySC();
	void clbkSetClassCaps(FILEHANDLE cfg);
	bool clbkLoadPanel2D(int id,PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
	static SURFHANDLE panel2dtex;
	static SURFHANDLE fontTex;
	bool clbkPanelMouseEvent(int id,int event,int mx,int my,void *context);
	bool clbkPanelRedrawEvent(int id,int event,SURFHANDLE surf,void *context);
	PanelElement *pel[2];
	void clbkMFDMode (int mfd, int mode);
private:	
	PROPELLANT_HANDLE mainTank;
	THRUSTER_HANDLE mainThr;
	MESHHANDLE hPanelMesh;
	double *a;
	void DefineMainPanel(PANELHANDLE hPanel);
	void ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH);
};

HINSTANCE g_hInst;
SURFHANDLE InterplanetarySC::panel2dtex = NULL;
SURFHANDLE InterplanetarySC::fontTex = NULL;
Logger systemLog("Spacecraft_Systemlog.txt");

InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel)
{
	hPanelMesh = NULL;
	for (int i = 0; i < 2; i++)
		pel[i] = new MFDButtonCol (this, i);
}

InterplanetarySC::~InterplanetarySC()
{
	if(hPanelMesh) oapiDeleteMesh(hPanelMesh);
	oapiDestroySurface(panel2dtex);
	oapiDestroySurface(fontTex);
	for (int i = 0; i < 2; i++)
		delete pel[i];
}

DLLCLBK void InitModule (HINSTANCE hModule)
{
	g_hInst = hModule;
	InterplanetarySC::panel2dtex = oapiLoadTexture("InterplanetarySC\\gimp.dds");
	InterplanetarySC::fontTex = oapiLoadTexture("InterplanetarySC\\font.dds");
	systemLog.logLine("Initialisiert");
	
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
	systemLog.logLine("Exit");
}

bool InterplanetarySC::clbkLoadPanel2D(int id,PANELHANDLE hPanel,
									   DWORD viewW, DWORD viewH)
{
	systemLog.logLine("Load2DPanel");
	oapiSetHUDMode(HUD_ORBIT);
	switch(id)
	{
	case 0:
		DefineMainPanel(hPanel);
		ScalePanel(hPanel, viewW,viewH);
		oapiSetHUDMode(HUD_NONE);
		return true;
	default:
		
		return false;
	}

}

void InterplanetarySC::DefineMainPanel(PANELHANDLE hPanel)
{
	systemLog.logLine("DefineMainPanel");
	static DWORD panelW = 1280;
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

	//MFD

	static NTVERTEX VTX_MFD[4] = {
    {214, 32,0,  0,0,0,  0,0},
    {514, 32,0,  0,0,0,  1,0},
    {214,332,0,  0,0,0,  0,1},
    {514,332,0,  0,0,0,  1,1}
  };
  static WORD IDX_MFD[6] = {
    0,1,2,
    3,2,1
  };
  MESHGROUP grp_mfd = {VTX_MFD, IDX_MFD, 4, 6, 0, 0, 0, 0, 0};
  oapiAddMeshGroup (hPanelMesh, &grp_mfd);
  RegisterPanelMFDGeometry (hPanel, MFD_LEFT, 0, 1);

   //int x0 = 40;
   //int x1 = 56;
   //int y0 = 300;
   //int y1 = 300+5*38;

   //int x2 = 40 + 380;
   //int x3 = 40+380+16;

   int x0 = 172;
   int x1 = 198;
   int y0 = 80;
   int y1 = 776-481;

   int x2 = 520;
   int x3 = 546;

  RegisterPanelArea (hPanel, AID_MFD_LBUTTONS, _R(x0,y0,x1,y1),
    PANEL_REDRAW_USER,
    PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY,
    panel2dtex, pel[0]);
  RegisterPanelArea (hPanel, AID_MFD_RBUTTONS, _R(x2,y0,x3,y1),
    PANEL_REDRAW_USER,
    PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY,
    panel2dtex, pel[1]);

 //   RegisterPanelArea (hPanel, AID_MFD_LBUTTONS, _R(x0,y0,x1,y1),
 //   PANEL_REDRAW_USER,
 //   PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY,
	//fontTex, pel[0]);
 // RegisterPanelArea (hPanel, AID_MFD_RBUTTONS, _R(x2,y0,x3,y1),
 //   PANEL_REDRAW_USER,
 //   PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY,
 //   fontTex, pel[1]);

}



void InterplanetarySC::ScalePanel(PANELHANDLE hPanel, DWORD viewW, DWORD viewH)
{
	systemLog.logLine("ScalePanel");
	double defscale = (double)viewW/1280.0;
	double magscale = max (defscale,1.0);
	SetPanelScaling(hPanel,defscale,magscale);
}

bool InterplanetarySC::clbkPanelMouseEvent(int id,int event,int mx,int my,void *context)
{
	std::string s = std::to_string(event)  +" - " + std::to_string(mx)  +" - "+ std::to_string(my);
	systemLog.logLine(s);
	systemLog.logLine("PENIS");
  if (context) {
	systemLog.logLine("CLICKED!");
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
	return pe->Redraw2D (surf,surf);
	//return pe->Redraw2D (fontTex);
  } else
    return false;
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
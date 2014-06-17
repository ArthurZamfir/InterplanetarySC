#define ORBITER_MODULE
#include "orbitersdk.h"
#include "OrbiterAPI.h"
#include "Logger.h"
#include "SubSystem.h"


void InitModule (HINSTANCE hModule)
{
	
}

class InterplanetarySC:public VESSEL3
{
public:
	InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel);
	void clbkSetClassCaps(FILEHANDLE cfg);
private:
	PROPELLANT_HANDLE mainTank;
	THRUSTER_HANDLE mainThr;
	double *a;
};

InterplanetarySC::InterplanetarySC(OBJHANDLE hObj,int fmodel):VESSEL3(hObj,fmodel)
{
	
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
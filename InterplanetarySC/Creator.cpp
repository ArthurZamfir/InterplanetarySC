#define ORBITER_MODULE
#include "orbitersdk.h"
#include "ThrusterFactory.h"
#include "VehicleAssembly.h"

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

	VehicleAssembly vab((VESSEL3) this);
	mainTank = CreatePropellantResource(8000.0);
	mainThr = CreateThruster(_V(0.0,0.0,-290.0),_V(0.0,0.0,1.0),800000.0,mainTank,8000.0);
	THRUSTER_HANDLE group1[1];
	group1[0] = mainThr;
	CreateThrusterGroup(group1,1,THGROUP_ATT_FORWARD);
	AddExhaust(mainThr,100,10); 

}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel,int flightmodel)
{
	VESSEL3* pVessel;
	pVessel = new InterplanetarySC(hvessel,flightmodel);
	return pVessel;
}
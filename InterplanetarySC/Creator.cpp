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
	
	//VehicleAssembly vab;
	//vab.CreatePropellantTanks();
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hvessel,int flightmodel)
{
	VESSEL3* pVessel;
	pVessel = new InterplanetarySC(hvessel,flightmodel);
	return pVessel;
}
#include "VehicleAssembly.h"
#include "Orbitersdk.h"
#include "ThrusterFactory.h"
#include "TankFactory.h"

VehicleAssembly::VehicleAssembly(VESSEL3 &vessel)
{
	defaultVessel = &vessel;
}

void VehicleAssembly::CreatePropellantTanks()
{
	TankFactory *factory = new TankFactory(*defaultVessel,800000);
    mainTank = factory->GenerateTank();
	//mainTank = defaultVessel->CreatePropellantResource(800000);
	//rcsTank = factory->GenerateTank(6000.0);
}

void VehicleAssembly::CreateThrusters()
{
	ThrusterFactory *factory = new ThrusterFactory(*defaultVessel,350000.0,8300,mainTank);

	THRUSTER_HANDLE *mainEngine = factory->GenerateThruster(_V(0,0,-290),_V(0,0,1));
	defaultVessel->CreateThrusterGroup(mainEngine,2,THGROUP_ATT_FORWARD);
	defaultVessel->AddExhaust(mainEngine,100,10);
}
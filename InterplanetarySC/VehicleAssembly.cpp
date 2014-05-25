#include "VehicleAssembly.h"
#include "Orbitersdk.h"
#include "ThrusterFactory.h"
#include "TankFactory.h"

void VehicleAssembly::CreatePropellantTanks(VESSEL3 &vessel)
{
	TankFactory *factory = new TankFactory(vessel,800000);
	mainTank = factory->GenerateTank();
	rcsTank = factory->GenerateTank(6000.0);
}

void VehicleAssembly::CreateThrusters(VESSEL3 &vessel)
{
	ThrusterFactory *factory = new ThrusterFactory(vessel,350000.0,8300,mainTank);

	factory->GenerateThruster(_V(0,0,-290),_V(0,0,1));
}
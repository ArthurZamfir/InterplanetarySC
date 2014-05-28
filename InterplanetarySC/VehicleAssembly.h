#include "Orbitersdk.h"

class VehicleAssembly
{
private:
	VESSEL3 *defaultVessel;
	PROPELLANT_HANDLE mainTank,rcsTank;
public:
	VehicleAssembly(VESSEL3 &vessel);
	void CreatePropellantTanks();
	void CreateThrusters();
};
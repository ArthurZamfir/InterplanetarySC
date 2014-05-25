#include "Orbitersdk.h"

class VehicleAssembly
{
private:
	PROPELLANT_HANDLE mainTank,rcsTank;
public:
	void CreatePropellantTanks(VESSEL3 &vessel);
	void CreateThrusters(VESSEL3 &vessel);
};